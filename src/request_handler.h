#pragma once
#define BOOST_BEAST_USE_STD_STRING_VIEW

#include <boost/json.hpp>
#include <boost/beast/http.hpp>
#include <filesystem>
#include <string_view>
#include <boost/asio.hpp>
#include <boost/asio/strand.hpp>
#include <memory>
#include <boost/beast.hpp>
#include <variant>
#include <optional>

#include "http_server.h"
#include "application.h"
#include "json_logger.h"
#include "extra_data.h"

namespace fs = std::filesystem;
using namespace std::literals;
namespace beast = boost::beast;
namespace http = beast::http;
using namespace boost::json;

using StringRequest = http::request<http::string_body>;
using StringResponse = http::response<http::string_body>;
using FileResponse = http::response<http::file_body>;

using Strand = boost::asio::strand<boost::asio::io_context::executor_type>;

namespace http_handler {

struct ContentType {
    ContentType() = delete;
    constexpr static std::string_view TEXT_HTML = "text/html"sv;
    constexpr static std::string_view TEXT_CSS = "text/css"sv;
    constexpr static std::string_view TEXT_PLAIN = "text/plain"sv;
    constexpr static std::string_view TEXT_JS = "text/javascript"sv;
    constexpr static std::string_view APPLICATION_JSON = "application/json"sv;
    constexpr static std::string_view APPLICATION_XML = "application/xml"sv;
    constexpr static std::string_view IMAGE_PNG = "image/png"sv;
    constexpr static std::string_view IMAGE_JPEG = "image/jpeg"sv;
    constexpr static std::string_view IMAGE_GIF = "image/gif"sv;
    constexpr static std::string_view IMAGE_BMP = "image/bmp"sv;
    constexpr static std::string_view IMAGE_VND_MICROSOFT_ICON = "image/vnd.microsoft.icon"sv;
    constexpr static std::string_view IMAGE_TIFF = "image/tiff"sv;
    constexpr static std::string_view IMAGE_SVG_XML = "image/svg+xml"sv;
    constexpr static std::string_view AUDIO_MPEG = "image/mpeg"sv;
    constexpr static std::string_view APPLICATION_OCTET_STREAM = "application/octet-stream"sv;
};

StringResponse MakeStringResponseGet(http::status status, std::string_view body, unsigned http_version,
    bool keep_alive,
    std::string_view content_type = ContentType::APPLICATION_JSON);

class ApiHandler {
public:
    explicit ApiHandler(app::Application& app, Strand api_strand, std::optional<int> auto_ticket, ExtraData& ex_data) 
    : app_{app}, api_strand_{api_strand}, move_dogs_timer_(api_strand_), auto_ticket_(auto_ticket), ex_data_{ex_data} {}

    StringResponse HandleStringRequest(const StringRequest& req);
private:
    StringResponse HandleJoinGame(const StringRequest& req) const;
    StringResponse HandleActionGame(const StringRequest& req) const;
    StringResponse HandleGetPlayers(const StringRequest& req) const;
    StringResponse HandleGetGameState(const StringRequest& req) const;
    StringResponse HandleGetMaps(const StringRequest& req) const;
    StringResponse HandleGetMapById(const StringRequest& req) const;
    StringResponse HandleMoveDogs(const StringRequest& req);
    StringResponse HandleGetRecords(const StringRequest& req) const;
    StringResponse HandleBadRequest (const StringRequest& req) const;

    app::Application& app_;
    Strand api_strand_;
    boost::asio::steady_timer move_dogs_timer_;
    std::optional<int> auto_ticket_;
    ExtraData& ex_data_;
};


class RequestHandler : public std::enable_shared_from_this<RequestHandler> {
public:
    using StaticFileResponse = std::variant<StringResponse, FileResponse>;

    explicit RequestHandler(Strand api_strand, model::Game& game, const char * static_file, app::Application& app, 
        std::optional<int> auto_ticket, ExtraData& ex_data)
        : api_strand_{api_strand}, game_{game}, static_file_(static_file), api_handler_{app, api_strand, auto_ticket, ex_data} {
    }

    RequestHandler(const RequestHandler&) = delete;
    RequestHandler& operator=(const RequestHandler&) = delete;

    template <typename Body, typename Allocator, typename Send>
    void operator()(http::request<Body, http::basic_fields<Allocator>>&& req, Send&& send, boost::asio::ip::tcp::endpoint endpoint) {
        auto start_time = std::chrono::steady_clock::now();

        LoggingRequest(req, endpoint);

        auto version = req.version();
        auto keep_alive = req.keep_alive();

        std::string target = std::string(req.target());
        if (target == "/favicon.ico") {
            StringResponse res;
            res.version(11);
            res.result(http::status::not_found);
            res.set(http::field::content_type, ContentType::TEXT_PLAIN);
            res.body() = "Error 404: Not Found";
            res.prepare_payload();

            auto end_time = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            JsonLogger::LogJson(static_cast<int>(duration.count()), 404, ContentType::TEXT_PLAIN);

            send(std::move(res));
        }

        if (target.rfind("/api/", 0) == 0)  {
            auto handle = [self = shared_from_this(), req = std::forward<decltype(req)>(req), send = std::forward<decltype(send)>(send), start_time] {
                assert(self->api_strand_.running_in_this_thread());
                auto response = self->HandleStringRequest(req);
    
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
                self->LoggingResponse(response, static_cast<int>(duration.count()));
    
                send(std::move(response));
            };
            boost::asio::dispatch(api_strand_, handle);
        } else {
            auto response = HandleStaticFileRequest(std::forward<decltype(req)>(req));

            std::visit([&send, start_time, this](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;
                
                auto end_time = std::chrono::steady_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                
                if constexpr (std::is_same_v<T, StringResponse>) {
                    LoggingResponse(arg, static_cast<int>(duration.count()));
                } else if constexpr (std::is_same_v<T, FileResponse>) {
                    LoggingResponse(arg, static_cast<int>(duration.count()));
                }
                
                send(std::move(arg));
            }, response);
        }
    }

private:
    StringResponse HandleStringRequest(const StringRequest& req);
    StaticFileResponse HandleStaticFileRequest(StringRequest&& req);

    template <typename Req>
    void LoggingRequest(const Req& req, boost::asio::ip::tcp::endpoint endpoint) {
        std::string_view uri = req.target();
        std::string_view method = boost::beast::http::to_string(req.method());

        JsonLogger::LogJson(uri, method, endpoint);
    }
    template <typename Res>
    void LoggingResponse(const Res& res, int duration) {
        int status_code = res.result_int();
        std::string_view content_type = res.find(http::field::content_type) != res.end()
                                    ? res[http::field::content_type]
                                    : "null";

        JsonLogger::LogJson(duration, status_code, content_type);
    }

    Strand api_strand_;
    model::Game& game_;
    const char * static_file_;
    ApiHandler api_handler_;
};

}  // namespace http_handler

