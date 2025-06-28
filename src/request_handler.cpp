#include "request_handler.h"
#include "json_utils.h"

#include <boost/beast.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <unordered_map>

namespace beast = boost::beast;
namespace http = beast::http;
namespace sys = boost::system;

namespace http_handler {
    std::unordered_map<std::string, std::string_view> content_types = {
        {"html", ContentType::TEXT_HTML}, {"htm", ContentType::TEXT_HTML},
        {"css", ContentType::TEXT_CSS},
        {"txt", ContentType::TEXT_PLAIN},
        {"js", ContentType::TEXT_JS},
        {"json", ContentType::APPLICATION_JSON},
        {"xml", ContentType::APPLICATION_XML},
        {"png", ContentType::IMAGE_PNG},
        {"jpg", ContentType::IMAGE_JPEG}, {"jpeg", ContentType::IMAGE_JPEG}, {"jpe", ContentType::IMAGE_JPEG},
        {"gif", ContentType::IMAGE_GIF},
        {"bmp", ContentType::IMAGE_BMP},
        {"ico", ContentType::IMAGE_VND_MICROSOFT_ICON},
        {"tiff", ContentType::IMAGE_TIFF}, {"tif", ContentType::IMAGE_TIFF},
        {"svg", ContentType::IMAGE_SVG_XML}, {"svgz", ContentType::IMAGE_SVG_XML},
        {"mp3", ContentType::AUDIO_MPEG}
    };
    
    std::string_view GetContentType(const std::string& extension) {
        if (auto it = content_types.find(extension); it != content_types.end()) {
            return it->second;
        }
        return ContentType::APPLICATION_OCTET_STREAM;
    }

    StringResponse MakeStringResponseGet(http::status status, std::string_view body, unsigned http_version,
        bool keep_alive, std::string_view content_type) {
        StringResponse response(status, http_version);
        response.set(http::field::content_type, content_type);
        response.body() = body;
        response.content_length(body.size());
        response.keep_alive(keep_alive);
        response.set(http::field::cache_control, "no-cache");
        return response;
    }

    StringResponse MakeErrorResponse(http::status status, const std::string& code, const std::string& message, 
    unsigned version, bool keep_alive) {
        boost::json::object error_object;
        error_object["code"] = code;
        error_object["message"] = message;
        return MakeStringResponseGet(status, serialize(error_object), version, keep_alive);
    }

    template <typename Request>
    bool ValidateToken(const Request& req, std::string& token, StringResponse& error_response) {
        auto auth_header = req[http::field::authorization];
        if (auth_header.empty() || auth_header.substr(0, 7) != "Bearer ") {
            error_response = MakeErrorResponse(http::status::unauthorized, "invalidToken", "Authorization header is missing",
                                               req.version(), req.keep_alive());
            return false;
        }

        token = std::string(auth_header).substr(7);
        if (token.length() != 32) {
            error_response = MakeErrorResponse(http::status::unauthorized, "invalidToken", "Invalid token",
                                               req.version(), req.keep_alive());
            return false;
        }

        return true;
    }

    StringResponse ApiHandler::HandleStringRequest(const StringRequest& req) {
        std::string_view target = req.target();
        
        if (target == "/api/v1/game/join") {
            return HandleJoinGame(req);
        } else if (target == "/api/v1/game/tick" && !auto_ticket_) {
            return HandleMoveDogs(req);
        } else if (target == "/api/v1/game/player/action") {
            return HandleActionGame(req);
        } else if (target == "/api/v1/game/players") {
            return HandleGetPlayers(req);
        } else if (target == "/api/v1/game/state" ) {
            return HandleGetGameState(req);
        } else if (target == "/api/v1/maps" || target == "/api/v1/maps/") {
            return HandleGetMaps(req);
        } else if (target.rfind("/api/v1/maps/", 0) == 0) {
            return HandleGetMapById(req);
        } else if (target.rfind("/api/v1/game/records", 0) == 0) {
            return HandleGetRecords(req);
        } else {
            return HandleBadRequest(req);
        }
    }

    StringResponse ApiHandler::HandleJoinGame(const StringRequest& req) const {
        const auto text_response = [&req](http::status status, std::string_view text) {
            return MakeStringResponseGet(status, text, req.version(), req.keep_alive());
        };

        if (req.method() != http::verb::post) {
            auto res = MakeErrorResponse(http::status::method_not_allowed, "invalidMethod", "Only POST method is expected",
                req.version(), req.keep_alive());
            res.set(http::field::allow, "POST");
            return res;
        }
    
        try {
            value json_body = parse(req.body());
            object& json_obj = json_body.as_object();

            if (!json_obj.contains("userName") || !json_obj.contains("mapId")) {
                return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Missing required fields",
                    req.version(), req.keep_alive());
            }
            
            std::string user_name = json_obj["userName"].as_string().c_str();

            if (user_name.empty()) {
                return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Invalid name",
                    req.version(), req.keep_alive());
            }

            std::string map_id = json_obj["mapId"].as_string().c_str();
            auto result = app_.GetJoinGameScenario()->Execute(user_name, map_id);

            if (!result) {
                return MakeErrorResponse(http::status::not_found, "mapNotFound", "Map not found", 
                    req.version(), req.keep_alive());
            }

            object response;
            response["authToken"] = result->token;
            response["playerId"] = result->player_id;
            
            auto res = text_response(http::status::ok, serialize(response));
            return res;
        } catch (const std::exception& e) {
            return MakeErrorResponse(http::status::bad_request, "invalidArgument", e.what(),
                req.version(), req.keep_alive());
        }
    }

    StringResponse ApiHandler::HandleMoveDogs(const StringRequest& req) {
        const auto text_response = [&req](http::status status, std::string_view text) {
            return MakeStringResponseGet(status, text, req.version(), req.keep_alive());
        };

        if (req.method() != http::verb::post) {
            auto res = MakeErrorResponse(http::status::method_not_allowed, "invalidMethod", "Only POST method is expected",
                req.version(), req.keep_alive());
            res.set(http::field::allow, "POST");
            return res;
        }

        try {
            value json_body = parse(req.body());

            if (!json_body.is_object() || !json_body.as_object().contains("timeDelta")) {
                return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Missing or invalid 'timeDelta' field",
                    req.version(), req.keep_alive());
            }
            
            std::chrono::milliseconds timeDelta = static_cast<std::chrono::milliseconds>(json_body.as_object().at("timeDelta").as_int64());

            app_.Tick(timeDelta);

            return text_response(http::status::ok, "{}");
        } catch (const std::exception& e) {
            return MakeErrorResponse(http::status::bad_request, "invalidArgument", e.what(),
                req.version(), req.keep_alive());
        }

    }

    StringResponse ApiHandler::HandleActionGame(const StringRequest& req) const {
        const auto text_response = [&req](http::status status, std::string_view text) {
            return MakeStringResponseGet(status, text, req.version(), req.keep_alive());
        };

        if (req[http::field::content_type].empty() || req[http::field::content_type] != "application/json") {
            return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Invalid content type",
                req.version(), req.keep_alive());
        }

        if (req.method() != http::verb::post) {
            auto res = MakeErrorResponse(http::status::method_not_allowed, "invalidMethod", "Only POST method is expected",
                req.version(), req.keep_alive());
            res.set(http::field::allow, "POST");
            return res;
        }

        std::string token;
        StringResponse error_response;
        if (!ValidateToken(req, token, error_response)) {
            return error_response;
        }
        try {
            value json_body = parse(req.body());
            object& json_obj = json_body.as_object();

            if (!json_obj.contains("move")) {
                return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Missing required fields",
                    req.version(), req.keep_alive());
            }
            
            std::optional<model::Direction> direction;
            std::string move = json_obj["move"].as_string().c_str();
            if (!move.empty()) {
                direction = model::StringToDirection(move);
                if (!direction) {
                    return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Invalid direction",
                                             req.version(), req.keep_alive());
                }
            }

            if(!app_.GetActionGameScenario()->Execute(token, direction)) {
                return MakeErrorResponse(http::status::unauthorized, "unknownToken", "Player token has not been found",
                    req.version(), req.keep_alive());
            }

            auto res = text_response(http::status::ok, "{}");
            return res;
        } catch (const std::exception& e) {
            return MakeErrorResponse(http::status::bad_request, "invalidArgument", e.what(),
                req.version(), req.keep_alive());
        }
    }

    StringResponse ApiHandler::HandleGetPlayers(const StringRequest& req) const {
        const auto text_response = [&req](http::status status, std::string_view text) {
            return MakeStringResponseGet(status, text, req.version(), req.keep_alive());
        };

        if (req.method() != http::verb::get && req.method() != http::verb::head) {
            auto res = MakeErrorResponse(http::status::method_not_allowed, "invalidMethod", "Only POST method is expected",
                req.version(), req.keep_alive());
            res.set(http::field::allow, "GET, HEAD");
            return res;
        }
    
        std::string token;
        StringResponse error_response;
        if (!ValidateToken(req, token, error_response)) {
            return error_response;
        }
        
        auto players = app_.GetPlayersScenario()->Execute(token);
        if (players.empty()) {
            return MakeErrorResponse(http::status::unauthorized, "unknownToken", "Player token has not been found",
                req.version(), req.keep_alive());
        }
    
        object response;
        for (const auto& player : players) {
            object player_object;
            player_object["name"] = player.name;
            response[std::to_string(player.id)] = std::move(player_object);
        }
        
        auto res = text_response(http::status::ok, serialize(response));
            return res;
    }

    double FormatDouble(double value) {
        double roundedNumber = std::round(value * 100.0) / 100.0;
        return roundedNumber;
    }

    StringResponse ApiHandler::HandleGetGameState (const StringRequest& req) const {
        const auto text_response = [&req](http::status status, std::string_view text) {
            return MakeStringResponseGet(status, text, req.version(), req.keep_alive());
        };

        if (req.method() != http::verb::get && req.method() != http::verb::head) {
            auto res = MakeErrorResponse(http::status::method_not_allowed, "invalidMethod", "Only POST method is expected",
                req.version(), req.keep_alive());
            res.set(http::field::allow, "GET, HEAD");
            return res;
        }

        std::string token;
        StringResponse error_response;
        if (!ValidateToken(req, token, error_response)) {
            return error_response;
        }

        auto [players, loots] = app_.GetGameStateScenario()->Execute(token);

        if (players.empty()) {
            return MakeErrorResponse(http::status::unauthorized, "unknownToken", "Player token has not been found",
                req.version(), req.keep_alive());
        }

        object players_json;
        for (const auto& player : players) {
            object player_obj;
            player_obj["pos"] = {FormatDouble(player.position.x), FormatDouble(player.position.y)};
            player_obj["speed"] = {FormatDouble(player.velocity.dx),FormatDouble(player.velocity.dy)};
            player_obj["dir"] = model::DirectionToString(player.direction);

            array bag_player;
            for (auto& [id, type] : player.bag) {
                object loot_obj;
                loot_obj["id"] = id;
                loot_obj["type"] = type;
                bag_player.push_back(loot_obj);
            }
            player_obj["bag"] = bag_player;

            player_obj["score"] = player.points;

            players_json[std::to_string(player.id)] = player_obj;
        }

        object loots_json;
        for (const auto& loot : loots) {
            boost::json::object loot_obj;
            loot_obj["type"] = loot.type;
            loot_obj["pos"] = {FormatDouble(loot.position.x), FormatDouble(loot.position.y)};
            loots_json[std::to_string(loot.id)] = loot_obj;
        }

        object response = {{"players", players_json}, {"lostObjects", loots_json}};

        auto res = text_response(http::status::ok, serialize(response));
            return res;
    }
    
    StringResponse ApiHandler::HandleGetMaps(const StringRequest& req) const {
        if (req.method() != http::verb::get && req.method() != http::verb::head) {
            auto res = MakeErrorResponse(http::status::method_not_allowed, "invalidMethod", "Only GET, HEAD method is expected",
                req.version(), req.keep_alive());
            res.set(http::field::allow, "GET, HEAD");
            return res;
        }

        auto maps = app_.GetMapsScenario()->Execute();

        array maps_json;
        for (const auto& map : maps) {
            boost::json::object obj;
            obj["id"] = map.id;
            obj["name"] = map.name;
            maps_json.push_back(obj);
        }
        return MakeStringResponseGet(http::status::ok, serialize(maps_json), req.version(), req.keep_alive());
    }

    StringResponse ApiHandler::HandleGetMapById(const StringRequest& req) const {
        if (req.method() != http::verb::get && req.method() != http::verb::head) {
            auto res = MakeErrorResponse(http::status::method_not_allowed, "invalidMethod", "Only GET, HEAD method is expected",
                req.version(), req.keep_alive());
            res.set(http::field::allow, "GET, HEAD");
            return res;
        }
        
        std::string map_id = std::string(req.target()).substr(13);
        auto map = app_.GetMapByIdScenario()->Execute(map_id);
    
        if (!map) {
            return MakeErrorResponse(http::status::not_found, "mapNotFound", "Map not found",
                req.version(), req.keep_alive());
        }

        auto map_json = json_utils::MapToJson(*map);
        map_json["lootTypes"] = ex_data_.GetLootsForMap(map_id);
        
        return MakeStringResponseGet(http::status::ok, serialize(map_json), req.version(), req.keep_alive());
    }

    StringResponse ApiHandler::HandleGetRecords(const StringRequest& req) const {
        const auto text_response = [&req](http::status status, std::string_view text) {
            return MakeStringResponseGet(status, text, req.version(), req.keep_alive());
        };

        if (req.method() != http::verb::get && req.method() != http::verb::head) {
            auto res = MakeErrorResponse(http::status::method_not_allowed, "invalidMethod", "Invalid method",
                                         req.version(), req.keep_alive());
            res.set(http::field::allow, "GET, HEAD");
            return res;
        }

        int start = 0;
        int max_items = 100;

        std::string target(req.target());
        if (target.find("?") != std::string::npos) {
            auto query = target.substr(target.find("?") + 1);
            std::unordered_map<std::string, std::string> params;
            std::istringstream query_stream(query);
            std::string param;
            while (std::getline(query_stream, param, '&')) {
                auto pos = param.find('=');
                if (pos != std::string::npos) {
                    params[param.substr(0, pos)] = param.substr(pos + 1);
                }
            }

            try {
                if (params.contains("start")) {
                    start = std::stoi(params["start"]);
                    if (start < 0) {
                        return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Start must be non-negative",
                                                 req.version(), req.keep_alive());
                    }
                }
                if (params.contains("maxItems")) {
                    max_items = std::stoi(params["maxItems"]);
                    if (max_items <= 0) {
                        return MakeErrorResponse(http::status::bad_request, "invalidArgument", "maxItems must be positive",
                                                 req.version(), req.keep_alive());
                    }
                }
            } catch (const std::exception& e) {
                return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Invalid query parameters",
                                         req.version(), req.keep_alive());
            }
        }

        try {
            auto records = app_.GetRecordsScenario()->Execute(start, max_items);
            array records_array;
            for (const auto& record : records) {
                object record_data;
                record_data["name"] = record.name;
                record_data["score"] = record.score;
                record_data["playTime"] = record.play_time_ms / 1000.0;
                records_array.emplace_back(record_data);
            }
            return text_response(http::status::ok, serialize(records_array));
        } catch (const std::invalid_argument& e) {
            return MakeErrorResponse(http::status::bad_request, "invalidArgument", e.what(),
                                     req.version(), req.keep_alive());
        } catch (const std::exception& e) {
            return MakeErrorResponse(http::status::bad_request, "invalidArgument", "Failed to retrieve records",
                                     req.version(), req.keep_alive());
        }
    }

    StringResponse ApiHandler::HandleBadRequest(const StringRequest& req) const {
        return MakeErrorResponse(http::status::bad_request, "badRequest", "Bad request",
            req.version(), req.keep_alive());
    }

    StringResponse RequestHandler::HandleStringRequest(const StringRequest& req) {
        return api_handler_.HandleStringRequest(req);
    }

    std::string ToLower(const std::string& str) {
        std::string lower_str = str;
        std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
        return lower_str;
    }

    std::string UrlDecode(const std::string& encoded) {
        std::ostringstream decoded;
        std::istringstream stream(encoded);
        char ch;
        
        while (stream.get(ch)) {
            if (ch == '%' && stream.peek() != EOF) {
                char hex1, hex2;
                stream.get(hex1);
                if (stream.get(hex2)) {
                    int value;
                    std::istringstream(std::string() + hex1 + hex2) >> std::hex >> value;
                    decoded << static_cast<char>(value);
                }
            } else if (ch == '+') {
                decoded << ' ';  
            } else {
                decoded << ch;
            }
        }
        
        return decoded.str();
    }

    bool IsSubPath(fs::path path, fs::path base) {
        path = fs::weakly_canonical(path);
        base = fs::weakly_canonical(base);
    
        for (auto b = base.begin(), p = path.begin(); b != base.end(); ++b, ++p) {
            if (p == path.end() || *p != *b) {
                return false;
            }
        }
        return true;
    }

    std::string GetFileExtension(const std::string_view filename) {
        size_t dot_pos = filename.rfind('.');
        
        if (dot_pos != std::string::npos && dot_pos != 0) {
            return ToLower(std::string(filename).substr(dot_pos + 1));
        }
        return "";

    }

    FileResponse MakeFileResponse(http::status status, http::file_body::value_type&& file,
        unsigned http_version, std::string_view content_type) {
        FileResponse response(status, http_version);
        response.set(http::field::content_type, content_type);
        response.body() = std::move(file);
        response.prepare_payload();
        return response;
        }

    RequestHandler::StaticFileResponse RequestHandler::HandleStaticFileRequest(StringRequest&& req) {
        //Поддержка файлов
        using namespace http;
        std::string request_path = static_file_ + UrlDecode(std::string(req.target()));

        if (fs::is_directory(request_path)) {
            request_path += "/index.html";
        }

        if (!IsSubPath(request_path, static_file_)) {
            auto res = MakeErrorResponse(http::status::bad_request, 
                "invalidPath", 
                "Access to the requested file is not allowed",
                req.version(), 
                req.keep_alive());
            res.set(http::field::content_type, ContentType::TEXT_PLAIN);
            return res;
        }

        std::string extension = GetFileExtension(request_path);
        std::string content_type = std::string(GetContentType(extension));

        file_body::value_type file;
        sys::error_code ec;
        file.open(request_path.c_str(), beast::file_mode::read, ec);

        if (ec) {
            auto res = MakeErrorResponse(http::status::not_found,
                                   "fileNotFound",
                                   "The requested file was not found",
                                   req.version(),
                                   req.keep_alive());
            res.set(http::field::content_type, ContentType::TEXT_PLAIN);
            return res;
        }
    

        return MakeFileResponse(http::status::ok, std::move(file), 11, content_type);
    }
}  // namespace http_handler



