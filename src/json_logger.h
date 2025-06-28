#pragma once

#include <boost/json.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/date_time.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio/error.hpp>
#include <string>
#include <string_view>
#include <map>

namespace logging = boost::log;
using namespace boost::json;
using namespace std::literals;
namespace beast = boost::beast;

class JsonLogger {
public:
    static void SetupLogging();

    static void LogJson(const boost::asio::ip::address& address, uint16_t port);
    static void LogJson(const std::map<std::string, std::string>& data);
    static void LogJson(const std::string_view uri, const std::string_view method, const boost::asio::ip::tcp::endpoint& endpoint);
    static void LogJson(int response_time, int status_code, const std::string_view content_type);
    static void LogJson(const beast::error_code& code, const std::string_view where);

private:
    static std::string GetTimeNow();

    static constexpr std::string_view start = "server started"sv;
    static constexpr std::string_view response = "response sent"sv;
    static constexpr std::string_view request = "request received"sv;
    static constexpr std::string_view exit = "server exited"sv;
    static constexpr std::string_view error = "error"sv;
};