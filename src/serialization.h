// serialization.h
#pragma once

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "model.h"
#include "application.h"

namespace model {

// Сериализация для model::Position
template <typename Archive>
void serialize(Archive& ar, model::Position& pos, [[maybe_unused]] const unsigned int version) {
    ar & pos.x;
    ar & pos.y;
}

// Сериализация для model::Velocity
template <typename Archive>
void serialize(Archive& ar, model::Velocity& vel, [[maybe_unused]] const unsigned int version) {
    ar & vel.dx;
    ar & vel.dy;
}

// Сериализация для model::Direction
template <typename Archive>
void serialize(Archive& ar, model::Direction& dir, [[maybe_unused]] const unsigned int version) {
    ar & reinterpret_cast<int&>(dir);
}

}

namespace serialization {

// Сериализация для model::Bag
class BagRepr {
public:
    BagRepr() = default;
    explicit BagRepr(const model::Bag& bag) 
        : capacity_(bag.GetCapacity()), items_(bag.GetItems()) {}

    model::Bag Restore() const {
        model::Bag bag(capacity_);
        for (const auto& [id, type] : items_) {
            bag.AddItem(id, type);
        }
        return bag;
    }

    template <typename Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
        ar & capacity_;
        ar & items_;
    }

private:
    int capacity_;
    std::unordered_map<int, int> items_;
};

// Сериализация для model::Dog
class DogRepr {
public:
    DogRepr() = default;
    explicit DogRepr(const model::Dog& dog)
        : name_(dog.GetName()),
          id_(dog.GetId()),
          bag_(dog.ConstGetBag()),
          position_(dog.GetPosition()),
          velocity_(dog.GetVelocity()),
          direction_(dog.GetDirection()),
          points_(dog.GetPoints()) {}

    model::Dog Restore() const {
        model::Bag bag = bag_.Restore();
        model::Dog dog(name_, id_, bag.GetCapacity(), position_, velocity_, direction_);
        
        for(auto& [id, type] : bag.GetItems()) {
            dog.GetBag().AddItem(id, type);
        }

        dog.AddPoints(points_);
        return dog;
    }

    template <typename Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
        ar & name_;
        ar & id_;
        ar & bag_;
        ar & position_;
        ar & velocity_;
        ar & direction_;
        ar & points_;
    }

private:
    std::string name_;
    uint32_t id_;
    BagRepr bag_;
    model::Position position_;
    model::Velocity velocity_;
    model::Direction direction_;
    int points_;
};

// Сериализация для model::Loot
class LootRepr {
public:
    LootRepr() = default;
    explicit LootRepr(const model::Loot& loot)
        : type_(loot.GetType()), pos_(loot.GetPosition()), id_(loot.GetId()) {}

    model::Loot Restore() const {
        return model::Loot(type_, pos_, id_);
    }

    template <typename Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
        ar & type_;
        ar & pos_;
        ar & id_;
    }

private:
    int type_;
    model::Position pos_;
    int id_;
};

// Сериализация для players::Player
class PlayerRepr {
public:
    PlayerRepr() = default;
    explicit PlayerRepr(const players::Player& player)
        : session_dog_id_(player.ConstGetDog()->GetId()),
          session_map_id_(*player.GetSession()->GetMap()->GetId()),
          token_(player.GetToken()),
          id_(player.GetId()) {}

    std::shared_ptr<players::Player> Restore(model::Game& game) const {
        auto session = game.GetSession(model::Map::Id(session_map_id_));
        if (!session) {
            throw std::runtime_error("Session not found during deserialization");
        }

        auto dog = session->GetDog(session_dog_id_);
        if (!dog) {
            throw std::runtime_error("Dog not found during deserialization");
        }

        auto player = std::make_shared<players::Player>(dog, session, id_);
        player->AddToken(token_);
        return player;
    }

    template <typename Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
        ar & session_dog_id_;
        ar & session_map_id_;
        ar & token_;
        ar & id_;
    }

private:
    uint32_t session_dog_id_;
    std::string session_map_id_;
    players::Token token_;
    uint32_t id_;
};

// Сериализация для players::Players
class PlayersRepr {
public:
    PlayersRepr() = default;
    explicit PlayersRepr(const players::Players& players) : next_id_player_(players.GetNextPlayerId()) {
        for (auto& player : players.GetPlayers()){
            players_.push_back(PlayerRepr(*player));
        }
    }

    void Restore(model::Game& game, players::Players& players) const {
        std::vector<std::shared_ptr<players::Player>> ready_players;

        for (auto& player : players_) {
            auto ready_player = player.Restore(game);
            players.AddPlayerWithToken(ready_player, ready_player->GetToken());
            ready_players.push_back(ready_player);
        }
        players.SetPlayers(std::move(ready_players));

        players.SetNextPlayerId(next_id_player_);
    }

    template <typename Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
        ar & players_;
        ar & next_id_player_;
    }

private:
    std::vector<PlayerRepr> players_;
    uint32_t next_id_player_;
};

// Сериализация для model::GameSession
class GameSessionRepr {
public:
    GameSessionRepr() = default;
    explicit GameSessionRepr(const model::GameSession& session) 
        : id_map_(*session.GetMap()->GetId()),
        next_id_dog_(session.GetNextIdDog()),
        next_id_loot_(session.GetNextIdLoot()) {
        for (auto& [id, dog] : session.GetDogs()){
            dogs_.push_back(DogRepr(*dog));
        }
        for (auto& [id, loot] : session.GetLoots()){
            loots_.push_back(LootRepr(*loot));
        }
    }

    model::GameSession Restore(model::Game& game) const {
        auto mapPtr = std::make_shared<model::Map>(*game.FindMap(model::Map::Id{id_map_}));
        model::GameSession session (mapPtr, game.GetSpawnPoints());
        std::unordered_map<uint32_t, std::shared_ptr<model::Dog>> ready_dogs;
        std::unordered_map<int, std::shared_ptr<model::Loot>> ready_loots;

        for (auto& dog : dogs_) {
            auto ready_dog = std::make_shared<model::Dog>(dog.Restore());
            ready_dogs[ready_dog->GetId()] = ready_dog;
        }
        for (auto& loot : loots_) {
            auto ready_loot = std::make_shared<model::Loot>(loot.Restore());
            ready_loots[ready_loot->GetId()] = ready_loot;
        }
        session.SetReadyDogs(std::move(ready_dogs));
        session.SetReadyLoots(std::move(ready_loots));
        session.SetNextIdDog(next_id_dog_);
        session.SetNextIdLoot(next_id_loot_);

        return session;
    }

    template <typename Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
        ar & dogs_;
        ar & loots_;
        ar & id_map_;
        ar & next_id_dog_;
        ar & next_id_loot_;
    }
    
private:
    std::vector<DogRepr> dogs_;
    std::vector<LootRepr> loots_;
    std::string id_map_;
    uint32_t next_id_dog_;
    int next_id_loot_;
};

class GameSessionsAndPlayersRepr {
public:
    GameSessionsAndPlayersRepr() = default;
    explicit GameSessionsAndPlayersRepr(const model::Game::GameSessions& sessions, const players::Players& players) 
    : players_(players) {
        for (auto& [id_map, session] : sessions){
            sessions_.push_back(GameSessionRepr(*session));
        }
    }

    void Restore(model::Game& game, players::Players& players) const {
        model::Game::GameSessions sessions;

        for (auto& session : sessions_) {
            auto ready_session = std::make_shared<model::GameSession>(session.Restore(game));
            sessions[ready_session->GetMap()->GetId()] = ready_session;
        }
        game.SetGameSessions(sessions);

        players_.Restore(game, players);
    }

    template <typename Archive>
    void serialize(Archive& ar, [[maybe_unused]] const unsigned int version) {
        ar & sessions_;
        ar & players_;
    }

private:
    std::vector<GameSessionRepr> sessions_;
    PlayersRepr players_;
};

} // namespace serialization