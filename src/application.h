#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <random>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <optional>
#include <vector>
#include <chrono> 

#include "model.h"
#include "collision_detector.h"
#include "extra_data.h"
#include "db_handler.h"

constexpr double WIDTH_PLAYER = 0.6;
constexpr double WIDTH_OFFICE = 0.5;
constexpr double WIDTH_LOOT = 0.0;

namespace players {

using Token = std::string;
    
class Player {
public:
    Player(std::shared_ptr<model::Dog> dog, std::shared_ptr<model::GameSession> session, uint32_t id)
        : dog_(std::move(dog)), session_(std::move(session)), id_(id) {}

    uint32_t GetPlayerDogId() const { return dog_->GetId(); }
    std::shared_ptr<model::Dog>& GetDog() { return dog_; }
    const std::shared_ptr<model::Dog>& ConstGetDog() const { return dog_; }
    const std::shared_ptr<model::GameSession>& GetSession() const { return session_; }
    void AddToken (const Token& token) { token_ = token; }
    Token GetToken () const { return token_; }
    uint32_t GetId () const { return id_; }

private:
    std::shared_ptr<model::Dog> dog_;
    std::shared_ptr<model::GameSession> session_;
    Token token_;
    uint32_t id_;
};
    
        
class PlayerTokens {
public:
    PlayerTokens() : generator1_(random_device_()), generator2_(random_device_()) {}

    Token AddPlayerToken(std::shared_ptr<Player> player);
    std::shared_ptr<Player> GetPlayerByToken(const Token& token);
    void AddPlayerWithToken(std::shared_ptr<Player> player, const Token& token) { tokens_[token] = player; }
    void RemovePlayerToken(const Token& token);

private:
    Token GenerateToken();

    std::unordered_map<Token, std::shared_ptr<Player>> tokens_;
    std::random_device random_device_;
    std::mt19937_64 generator1_;
    std::mt19937_64 generator2_;
};
    
class Players {
public:
    Players() : tokens_(std::make_shared<PlayerTokens>()) {}

    std::shared_ptr<Player> AddPlayer(std::shared_ptr<model::Dog> dog, std::shared_ptr<model::GameSession> session);
    std::shared_ptr<Player> GetPlayerById(uint32_t dog_id, const std::string& map_id);
    std::shared_ptr<Player> GetPlayerByToken(const Token& token);
    void RemovePlayer(std::shared_ptr<Player> player);

    const auto& GetPlayers() const { return players_; }
    uint32_t GetNextPlayerId() const { return next_id_player_; }
    void SetPlayers(std::vector<std::shared_ptr<Player>>&& players) {
        players_ = std::move(players);
    }
    void SetNextPlayerId(uint32_t id) { next_id_player_ = id; }

    void AddPlayerWithToken (std::shared_ptr<Player> player, const Token& token) { tokens_->AddPlayerWithToken(player, token); }


private:
    std::vector<std::shared_ptr<Player>> players_;
    std::shared_ptr<PlayerTokens> tokens_;
    uint32_t next_id_player_ = 0;
};

}// namespace players

namespace app {

class JoinGameScenario {
public:
    struct Result {
        uint32_t player_id;
        std::string token;
    };

    JoinGameScenario(model::Game& game, players::Players& players)
        : game_(game), players_(players) {}

    std::optional<Result> Execute(const std::string& user_name, const std::string& map_id);

private:
    model::Game& game_;
    players::Players& players_;
};

class ActionGameScenario {
public:
    ActionGameScenario(model::Game& game, players::Players& players)
        : game_(game), players_(players) {}

    bool Execute(const players::Token& token, std::optional<model::Direction> direction);
private:
    model::Game& game_;
    players::Players& players_;
};

class PlayersScenario {
public:
    struct PlayerData {
        uint32_t id;
        std::string name;
    };

    PlayersScenario(players::Players& players)
        : players_(players) {}

    std::vector<PlayerData> Execute(const players::Token& token);

private:
    players::Players& players_;
};

class GameStateScenario {
public:
    struct PlayerData {
        uint32_t id;
        model::Position position;
        model::Velocity velocity;
        model::Direction direction;
        std::unordered_map<int, int> bag;
        int points = 0;
    };

    struct LootData {
        int id;
        int type;
        model::Position position;
    };

    GameStateScenario(players::Players& players)
        : players_(players) {}

    std::pair<std::vector<PlayerData>, std::vector<LootData>> Execute(const players::Token& token);

private:
    players::Players& players_;
};

class MapsScenario {
public:
    struct MapData {
        std::string id;
        std::string name;
    };

    explicit MapsScenario(model::Game& game) : game_(game) {}

    std::vector<MapData> Execute();

private:
    model::Game& game_;
};

class MapByIdScenario {
public:
    explicit MapByIdScenario(model::Game& game) : game_(game) {}

    const model::Map* Execute(const std::string& id_map);

private:
    model::Game& game_;
};

class MoveDogsScenario {
public:
    MoveDogsScenario(model::Game& game, ExtraData& ex_data, players::Players& players, DbHandler& db_handler, double dog_retirement_time) 
    : game_(game), ex_data_(ex_data), players_(players), db_handler_(db_handler), dog_retirement_time_(dog_retirement_time) {}

    void Execute(std::chrono::milliseconds delta);

private:
model::Position MoveDog(model::Dog& dog, const std::shared_ptr<model::Map>& map, double delta_time);

    model::Game& game_;
    ExtraData& ex_data_;
    players::Players& players_;
    DbHandler& db_handler_;
    double dog_retirement_time_;
};

class RecordsScenario {
public:
    RecordsScenario(DbHandler& db_handler) : db_handler_(db_handler) {}

    std::vector<RetiredPlayer> Execute(int start, int max_items);

private:
    DbHandler& db_handler_;
};

class ApplicationListener {
public:
    virtual ~ApplicationListener() = default;
    virtual void OnTick(std::chrono::milliseconds delta) = 0;
    virtual void OnShutdown() = 0;
};

class Application {
public:
    Application(model::Game& game, ExtraData& ex_data, DbHandler& db_handler, double dog_retirement_time) 
    : game_(game), ex_data_(ex_data), db_handler_(db_handler), dog_retirement_time_(dog_retirement_time) {}

    std::shared_ptr<JoinGameScenario> GetJoinGameScenario();
    std::shared_ptr<ActionGameScenario> GetActionGameScenario();
    std::shared_ptr<PlayersScenario> GetPlayersScenario();
    std::shared_ptr<GameStateScenario> GetGameStateScenario();
    std::shared_ptr<MoveDogsScenario> GetMoveDogsScenario();
    std::shared_ptr<MapsScenario> GetMapsScenario();
    std::shared_ptr<MapByIdScenario> GetMapByIdScenario();
    std::shared_ptr<RecordsScenario> GetRecordsScenario();

    void Tick(std::chrono::milliseconds delta);
    players::Players& GetPlayers() { return players_; }

    void AddListener(std::shared_ptr<ApplicationListener> listener) {
        listeners_.push_back(listener);
    }

private:
    model::Game& game_;
    players::Players players_;
    ExtraData& ex_data_;
    DbHandler& db_handler_;
    double dog_retirement_time_;
    std::vector<std::shared_ptr<ApplicationListener>> listeners_;
};

}