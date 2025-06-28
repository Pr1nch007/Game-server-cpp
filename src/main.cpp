#include "sdk.h"

#define BOOST_BEAST_USE_STD_STRING_VIEW

#include <boost/program_options.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <thread>
#include <fstream>
#include <optional>
#include <vector>
#include <cstdlib>
#include <stdexcept>

#include "json_loader.h"
#include "request_handler.h"
#include "json_logger.h"
#include "model.h"
#include "application.h"
#include "ticker.h"
#include "extra_data.h"
#include "serializing_listener.h"
#include "db_handler.h"

namespace net = boost::asio;
namespace sys = boost::system;

using namespace std::literals;

struct Args {
    std::optional<int> tick_period;
    std::filesystem::path config_file;
    std::string www_root;
    bool randomize_spawn_points = false;
    std::filesystem::path state_file;
    std::optional<int> save_state_period;
};

[[nodiscard]] std::optional<Args> ParseCommandLine(int argc, const char* const argv[]) {
    namespace po = boost::program_options;

    po::options_description desc{"All options"s};

    Args args;
    desc.add_options()
        ("help,h", "Show help")
        ("tick-period,t", po::value<int>()->value_name("milliseconds"), "set tick period")
        ("config-file,c", po::value(&args.config_file)->required()->value_name("file"), "set config file path")
        ("www-root,w", po::value(&args.www_root)->required()->value_name("dir"), "set static files root")
        ("randomize-spawn-points", po::bool_switch(&args.randomize_spawn_points), "spawn dogs at random positions")
        ("state-file,s", po::value(&args.state_file)->value_name("file"), "set state file path")
        ("save-state-period,p", po::value<int>()->value_name("milliseconds"), "set state save period");

        
        
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help"s)) {
        std::cout << desc;
        return std::nullopt;
    }

    if (vm.contains("tick-period")) {
        args.tick_period = vm["tick-period"].as<int>();
        if (*args.tick_period <= 0) {
            throw po::error("tick period must be positive");
        }
    }

    if (!vm.contains("config-file"s)) {
        throw std::runtime_error("No have config-file"s);
    }
    if (!vm.contains("www-root"s)) {
        throw std::runtime_error("No have www-root"s);
    }

    if (vm.contains("save-state-period")) {
        args.save_state_period = vm["save-state-period"].as<int>();
        if (*args.save_state_period <= 0) {
            throw po::error("save state period must be positive");
        }
    }

    return args;
}



namespace {
template <typename Fn>
void RunWorkers(unsigned n, const Fn& fn) {
    n = std::max(1u, n);
    std::vector<std::jthread> workers;
    workers.reserve(n - 1);
    while (--n) {
        workers.emplace_back(fn);
    }
    fn();
}

}  // namespace

int main(int argc, const char* argv[]) {
    try {
        auto args = ParseCommandLine(argc, argv);
        if (!args) {
            return EXIT_SUCCESS;
        }

        // Получаем URL базы данных из переменной окружения
        const char* db_url = std::getenv("GAME_DB_URL");
        if (!db_url) {
            throw std::runtime_error("GAME_DB_URL environment variable not set");
        }

        // Инициализация базы данных
        DbHandler db_handler(db_url);
        try {
            db_handler.InitializeDatabase();
        } catch (const std::exception& e) {
            std::cerr << "Failed to initialize database: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        //Загружаем карту из файла и построить модель игры
        ExtraData ex_data;
        double dog_retirement_time = 0.0;
        model::Game game = json_loader::LoadGame(args->config_file, args->randomize_spawn_points, ex_data, dog_retirement_time);
        app::Application app(game, ex_data, db_handler, dog_retirement_time);

        auto save_period = args->save_state_period.value_or(0);
        auto serializer = std::make_shared<infrastructure::SerializingListener>(
            args->state_file,
            std::chrono::milliseconds(save_period),
            app.GetPlayers(),
            game
        );
        app.AddListener(serializer);

        if (!args->state_file.empty()) {
            if (!serializer->TryRestoreState()) {
                if (fs::exists(args->state_file)) {
                    std::cerr << "Failed to restore state from file: " << args->state_file << std::endl;
                    return EXIT_FAILURE;
                }
            }
        }

        //Инициализируем io_context
        const unsigned num_threads = std::thread::hardware_concurrency();
        net::io_context ioc(num_threads);

        auto api_strand = net::make_strand(ioc);

        //Добавляем асинхронный обработчик сигналов SIGINT и SIGTERM
        net::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc, &serializer](const sys::error_code& ec, [[maybe_unused]] int signal_number) {
            if (!ec) {
                ioc.stop();
                serializer->OnShutdown();
            }
        });

        if(args->tick_period) {
            auto ticker = std::make_shared<Ticker>(api_strand, std::chrono::milliseconds(args->tick_period.value()),
                [&app](std::chrono::milliseconds delta) { app.Tick(delta); }
            );
            ticker->Start();
        }

        //Создаём обработчик HTTP-запросов и связываем его с моделью игры
        auto handler = std::make_shared<http_handler::RequestHandler>(api_strand, game, args->www_root.c_str(), 
        app, args->tick_period, ex_data);

        //Запустить обработчик HTTP-запросов, делегируя их обработчику запросов
        const auto address = net::ip::make_address("0.0.0.0");
        constexpr net::ip::port_type port = 8080;
        http_server::ServeHttp(ioc, {address, port}, [handler](auto&& req, auto&& send, auto&& endpoint) {
            (*handler)(std::forward<decltype(req)>(req), std::forward<decltype(send)>(send), std::forward<decltype(endpoint)>(endpoint));
        });

        //Логируем старт сервера
        JsonLogger::SetupLogging();
        JsonLogger::LogJson(address, port);

        // Запускаем обработку асинхронных операций
        RunWorkers(std::max(1u, num_threads), [&ioc] {
            ioc.run();
        });
    } catch (const std::exception& ex) {
        std::map<std::string, std::string> data{
            {"code", std::to_string(EXIT_FAILURE)},
            {"exception", ex.what()}
        };
        JsonLogger::LogJson(data);
        return EXIT_FAILURE;
    }
    std::map<std::string, std::string> data{
        {"code", "0"}
    };
    JsonLogger::LogJson(data);
}
