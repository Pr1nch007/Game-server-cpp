#define BOOST_BEAST_USE_STD_STRING_VIEW

#include "json_logger.h"
#include <boost/log/expressions.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <iostream>

void JsonLogger::SetupLogging() {
    logging::add_common_attributes();
    logging::add_console_log(
        std::cout,
        boost::log::keywords::format = "%Message%",
        boost::log::keywords::auto_flush = true
    );
}

std::string JsonLogger::GetTimeNow() {
    boost::posix_time::ptime current_time = boost::posix_time::second_clock::local_time();
    return boost::posix_time::to_iso_extended_string(current_time);
}

void JsonLogger::LogJson(const boost::asio::ip::address& address, uint16_t port) {
    boost::json::object log_entry;
    log_entry["timestamp"] = GetTimeNow();

    boost::json::object data;
    data["address"] = address.to_string();
    data["port"] = port;

    log_entry["data"] = data;
    log_entry["message"] = std::string(start);

    BOOST_LOG_TRIVIAL(info) << boost::json::serialize(log_entry);
}

void JsonLogger::LogJson(const std::map<std::string, std::string>& data) {
    boost::json::object log;
    log["timestamp"] = GetTimeNow();

    boost::json::object jsonData;
    for (const auto& [key, value] : data) {
        jsonData[key] = value;
    }

    log["data"] = jsonData;
    log["message"] = std::string(exit);

    BOOST_LOG_TRIVIAL(info) << boost::json::serialize(log);
}

void JsonLogger::LogJson(const std::string_view uri, const std::string_view method, const boost::asio::ip::tcp::endpoint& endpoint) {
    boost::json::object log;
    log["timestamp"] = GetTimeNow();

    std::string ip = endpoint.address().to_string();

    boost::json::object data;
    data["ip"] = ip;
    data["URI"] = std::string(uri);
    data["method"] = std::string(method);

    log["data"] = std::move(data);
    log["message"] = std::string(request);

    BOOST_LOG_TRIVIAL(info) << boost::json::serialize(log);
}

void JsonLogger::LogJson(int response_time, int status_code, const std::string_view content_type) {
    boost::json::object log;
    log["timestamp"] = GetTimeNow();

    boost::json::object data;
        data["response_time"] = response_time;
        data["code"] = status_code;
        data["content_type"] = std::string(content_type);

    log["data"] = data;
    log["message"] = std::string(response);

    BOOST_LOG_TRIVIAL(info) << boost::json::serialize(log);
}

void JsonLogger::LogJson(const beast::error_code& code, const std::string_view where) {
    boost::json::object log;
    log["timestamp"] = GetTimeNow();

    boost::json::object data;
    data["code"] = code.value();
    data["text"] = code.message();
    data["where"] = std::string(where);

    log["data"] = std::move(data);
    log["message"] = std::string(error);

    BOOST_LOG_TRIVIAL(error) << boost::json::serialize(log);
}