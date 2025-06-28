#include "application.h"

#include <unordered_set>

namespace players {

    Token PlayerTokens::AddPlayerToken(std::shared_ptr<Player> player) {
        Token token = GenerateToken();
        tokens_[token] = player;
        return token;
    }

    std::shared_ptr<Player> PlayerTokens::GetPlayerByToken(const Token& token) {
        auto it = tokens_.find(token);
        return it != tokens_.end() ? it->second : nullptr;
    }

    void PlayerTokens::RemovePlayerToken(const Token& token) {
        tokens_.erase(token);
    }

    Token PlayerTokens::GenerateToken() {
        std::uniform_int_distribution<uint64_t> dist;
        uint64_t part1 = dist(generator1_);
        uint64_t part2 = dist(generator2_);
        
        std::stringstream ss;
        ss << std::hex << part1 << part2;
        std::string token = ss.str();

        if (token.length() < 32) {
            token.insert(0, 32 - token.length(), '0');
        } else if (token.length() > 32) {
            token = token.substr(0, 32);
        }

        return token;
    }

    std::shared_ptr<Player> Players::AddPlayer(std::shared_ptr<model::Dog> dog, std::shared_ptr<model::GameSession> session) {
        auto player = std::make_shared<Player>(dog, session, next_id_player_++);
        Token auth_token = tokens_->AddPlayerToken(player);
        player->AddToken(auth_token);
        players_.push_back(player);
        return player;
    }

    std::shared_ptr<Player> Players::GetPlayerById(uint32_t dog_id, const std::string& map_id) {
        for (const auto& player : players_) {
            if (player->GetPlayerDogId() == dog_id && *player->GetSession()->GetMap()->GetId() == map_id) {
                return player;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Player> Players::GetPlayerByToken(const Token& token) {
        return tokens_->GetPlayerByToken(token);
    }

    void Players::RemovePlayer(std::shared_ptr<Player> player) {
        tokens_->RemovePlayerToken(player->GetToken());
        players_.erase(std::remove(players_.begin(), players_.end(), player), players_.end());
    }

}

namespace app {

    std::optional<JoinGameScenario::Result> JoinGameScenario::Execute(const std::string& user_name, const std::string& map_id) {
        auto map = game_.FindMap(model::Map::Id{map_id});
        if (!map) {
            return std::nullopt;
        }

        auto session = game_.GetSession(model::Map::Id{map_id});
        if (!session) {
            std::cerr << "Error: failed to get game session!" << std::endl;
            return std::nullopt;
        }

        auto bag_capacity = game_.GetDefaultBagCapacity(model::Map::Id{map_id});
        auto new_dog = session->AddDog(user_name, bag_capacity);
        if (!new_dog ) {
            std::cerr << "Error: не создалась собака!" << std::endl;
        }

        auto player = players_.AddPlayer(new_dog, session);

        return Result{player->GetId(), player->GetToken()};
    }

    bool ActionGameScenario::Execute(const players::Token& token, std::optional<model::Direction> direction) {
        auto player = players_.GetPlayerByToken(token);
        if (!player) {
            return false;
        }
        auto session = player->GetSession();
        if (!session) {
            std::cerr << "Error: player has no active session!" << std::endl;
            return false;
        }
        auto id_map = session->GetMap()->GetId();
        auto speed = game_.GetDefaultDogSpeed(id_map);

        if (!direction) {
            player->GetDog()->SetVelocity({0.0, 0.0});
        } else {
            switch (direction.value()) {
                case model::Direction::WEST: 
                    player->GetDog()->SetDirection(model::Direction::WEST);
                    player->GetDog()->SetVelocity({-speed, 0.0});
                    break;
                case model::Direction::EAST:
                    player->GetDog()->SetDirection(model::Direction::EAST);
                    player->GetDog()->SetVelocity({speed, 0.0});
                    break;
                case model::Direction::NORTH: 
                    player->GetDog()->SetDirection(model::Direction::NORTH);
                    player->GetDog()->SetVelocity({0.0, -speed});
                    break;
                case model::Direction::SOUTH: 
                    player->GetDog()->SetDirection(model::Direction::SOUTH);
                    player->GetDog()->SetVelocity({0.0, speed});
                    break;
            }
        }
        player->GetDog()->UpdateTimes(0);
        return true;
    }

    std::vector<PlayersScenario::PlayerData> PlayersScenario::Execute(const players::Token& token) {
        auto player = players_.GetPlayerByToken(token);
        if (!player) {
            return {};
        }
        auto session = player->GetSession();
        auto dogs = session->GetDogs();
        std::vector<PlayerData> result;
        result.reserve(dogs.size());

        std::transform(dogs.begin(), dogs.end(), std::back_inserter(result),
            [](const auto& pair) {
                const auto& [id, dog] = pair;
                return PlayerData{id, dog->GetName()};
            });
        return result;
    }

    std::pair<std::vector<GameStateScenario::PlayerData>, std::vector<GameStateScenario::LootData>> GameStateScenario::Execute(const players::Token& token) {
        auto player = players_.GetPlayerByToken(token);
        if (!player) {
            return {{},{}};
        }
        auto session = player->GetSession();
        auto dogs = session->GetDogs();
        std::vector<PlayerData> players_data;
        players_data.reserve(dogs.size());

        std::transform(dogs.begin(), dogs.end(), std::back_inserter(players_data),
            [](const auto& pair) {
                const auto& [id, dog] = pair;
                return PlayerData{
                    id,
                    dog->GetPosition(),
                    dog->GetVelocity(),
                    dog->GetDirection(),
                    dog->GetBag().GetItems(),
                    dog->GetPoints()
                };
            });

        auto loots = session->GetLoots();
        std::vector<LootData> loots_data;
        loots_data.reserve(loots.size());

        std::transform(loots.begin(), loots.end(), std::back_inserter(loots_data),
            [](const auto& pair) {
                const auto& [id, loot] = pair;
                return LootData{
                    id,
                    loot->GetType(),
                    loot->GetPosition()
                };
            });
        return {players_data, loots_data};
    }

    std::vector<MapsScenario::MapData> MapsScenario::Execute() {
        std::vector<MapData> result;
        for (const auto& map : game_.GetMaps()) {
            result.push_back({*map.GetId(), map.GetName()});
        }
        return result;
    }

    const model::Map* MapByIdScenario::Execute(const std::string& id_map) {
        auto map = game_.FindMap(model::Map::Id{id_map});
        return map;
    }

    void MoveDogsScenario::Execute(std::chrono::milliseconds delta) {
        double delta_time_sec = static_cast<double>(delta.count()) / 1000.0;
        int64_t delta_ms = delta.count();
        
        auto sessions = game_.GetSessions();
        for (const auto& [id_map, session] : sessions) {
            auto dogs = session->GetDogs();
            auto loots_gener = game_.GetLootGenerator();
            int num_loots = loots_gener->Generate(delta, session->GetNumLoots(), dogs.size());
            session->AddLoots(num_loots);

            auto loots = session->GetLoots();
            auto offices = session->GetMap()->GetOffices();

            std::vector<collision_detector::Item> items;
            items.reserve(loots.size() + offices.size());

            std::transform(loots.begin(), loots.end(), std::back_inserter(items),
                [](const auto& pair) {
                    const auto& [id, loot] = pair;
                    return collision_detector::Item{
                        {loot->GetPosition().x, loot->GetPosition().y},
                        WIDTH_LOOT / 2,
                        id + 1
                    };
                });

            std::transform(offices.begin(), offices.end(), std::back_inserter(items),
                [](const auto& office) {
                    return collision_detector::Item{
                        {static_cast<double>(office.GetPosition().x), static_cast<double>(office.GetPosition().y)},
                        WIDTH_OFFICE / 2,
                        0
                    };
                });

            std::vector<collision_detector::Gatherer> gatherers;
            gatherers.reserve(dogs.size());

            std::unordered_set<uint32_t> standing_dogs;

            for (auto& [id, dog] : dogs) {
                auto map = session->GetMap();
                collision_detector::Gatherer gatherer;
                gatherer.start_pos = {dog->GetPosition().x, dog->GetPosition().y};
                auto prev_velocity = dog->GetVelocity();
                auto end_pos = MoveDog(*dog, map, delta_time_sec);
                gatherer.end_pos = {end_pos.x, end_pos.y};
                gatherer.width = WIDTH_PLAYER / 2;
                gatherer.id = id;
                gatherers.push_back(gatherer);

                if (prev_velocity.IsZero() && dog->GetVelocity().IsZero()) {
                    standing_dogs.insert(id);
                }
            }

            collision_detector::Provider provider(items, gatherers);
            auto events = collision_detector::FindGatherEvents(provider);
            std::unordered_set<size_t> collected_loot_ids;

            for (const auto& event : events) {
                auto dog = dogs.at(event.gatherer_id);
                if (event.item_id == 0) {
                    auto loots_in_bag = dog->GetBag().GetItems();
                    int points = 0;
                    for (auto& [id, type] : loots_in_bag) {
                        points += ex_data_.GetValueLoot(*id_map, type);
                    }
                    dog->AddPoints(points);
                    dog->GetBag().Clear();
                } else if (collected_loot_ids.count(event.item_id) == 0) {
                    auto loot = loots.at(event.item_id - 1);
                    if (!dog->GetBag().IsFull()) {
                        dog->GetBag().AddItem(event.item_id - 1, loot->GetType());
                        session->RemoveLoot(event.item_id - 1);
                        collected_loot_ids.insert(event.item_id);
                    }
                }
            }
            for (auto it = dogs.begin(); it != dogs.end();) {
                auto& [id, dog] = *it;

                if(standing_dogs.count(id)) {
                    dog->AddInactiveTime(delta_ms);
                } else {
                    dog->UpdateTimes(delta_ms);
                }

                if (dog->GetInactiveTimeMs() >= dog_retirement_time_ * 1000) {
                    auto player = players_.GetPlayerById(id, *session->GetMap()->GetId());
                    if (player) {
                        db_handler_.SaveRetiredPlayer({
                            dog->GetName(),
                            dog->GetPoints(),
                            dog->GetJoinTimeMs()
                        });
                        players_.RemovePlayer(player);
                        session->RemoveDog(id);
                        continue;
                    }
                }
                ++it;            
            }
        }
    }

    model::Position MoveDogsScenario::MoveDog(model::Dog& dog, const std::shared_ptr<model::Map>& map, double delta_time) {
        constexpr int BINARY_SEARCH_ITERATIONS = 15;

        if (dog.GetVelocity().IsZero()) return dog.GetPosition();

        const double speed_x = dog.GetVelocity().dx;
        const double speed_y = dog.GetVelocity().dy;
        const double move_x = speed_x * delta_time;
        const double move_y = speed_y * delta_time;

        auto findBoundary = [&map](model::Position start, model::Position end) -> model::Position {
            const int steps = 100;
            model::Position current = start;
            model::Position last_valid = start;

            for (int i = 0; i <= steps; ++i) {
                double ratio = static_cast<double>(i) / steps;
                current.x = start.x + (end.x - start.x) * ratio;
                current.y = start.y + (end.y - start.y) * ratio;

                if (map->IsOnRoad(current)) {
                    last_valid = current;
                } else {
                    model::Position low = last_valid;
                    model::Position high = current;
                    for (int j = 0; j < BINARY_SEARCH_ITERATIONS; ++j) {
                        model::Position mid {
                            (low.x + high.x) / 2,
                            (low.y + high.y) / 2
                        };
                        if (map->IsOnRoad(mid)) {
                            low = mid;
                        } else {
                            high = mid;
                        }
                    }
                    if (std::abs(start.x - end.x) > std::abs(start.y - end.y)) {
                        low.x = std::round(low.x * 10) / 10;
                    } else {
                        low.y = std::round(low.y * 10) / 10;
                    }
                    return low;
                }
            }
            return last_valid;
        };
        model::Position new_pos = {
            dog.GetPosition().x + move_x,
            dog.GetPosition().y + move_y
        };

        model::Position final_pos;

        if (map->IsOnRoad(new_pos)) {
            dog.SetPosition(new_pos);
            final_pos = new_pos;
        } else {
            model::Position boundary = findBoundary(dog.GetPosition(), new_pos);
            if (speed_x > 0) {
                boundary.x = std::floor(boundary.x * 10 + 0.5) / 10;
            } else if (speed_x < 0) {
                boundary.x = std::ceil(boundary.x * 10 - 0.5) / 10;
            }

            if (speed_y > 0) {
                boundary.y = std::floor(boundary.y * 10 + 0.5) / 10;
            } else if (speed_y < 0) {
                boundary.y = std::ceil(boundary.y * 10 - 0.5) / 10;
            }

            dog.SetPosition(boundary);
            dog.SetVelocity({0.0, 0.0});
            final_pos = boundary;
        }

        return final_pos;
    }

    std::vector<RetiredPlayer> RecordsScenario::Execute(int start, int max_items) {
        if (max_items > 100) {
            throw std::invalid_argument("maxItems exceeds maximum allowed value of 100");
        }
        return db_handler_.GetRecords(start, max_items);
    }

    std::shared_ptr<JoinGameScenario> Application::GetJoinGameScenario() {
        return std::make_shared<JoinGameScenario>(game_, players_);
    }

    std::shared_ptr<ActionGameScenario> Application::GetActionGameScenario() {
        return std::make_shared<ActionGameScenario>(game_, players_);
    }

    std::shared_ptr<PlayersScenario> Application::GetPlayersScenario() {
        return std::make_shared<PlayersScenario>(players_);
    }

    std::shared_ptr<GameStateScenario> Application::GetGameStateScenario() {
        return std::make_shared<GameStateScenario>(players_);
    }

    std::shared_ptr<MoveDogsScenario> Application::GetMoveDogsScenario() {
        return std::make_shared<MoveDogsScenario>(game_, ex_data_, players_, db_handler_, dog_retirement_time_);
    }

    std::shared_ptr<MapsScenario> Application::GetMapsScenario() {
        return std::make_shared<MapsScenario>(game_);
    }
    std::shared_ptr<MapByIdScenario> Application::GetMapByIdScenario() {
        return std::make_shared<MapByIdScenario>(game_);
    }
    std::shared_ptr<RecordsScenario> Application::GetRecordsScenario() {
        return std::make_shared<RecordsScenario>(db_handler_);
    }

    void Application::Tick(std::chrono::milliseconds delta) {
        auto scenario = GetMoveDogsScenario();
        scenario->Execute(delta);
        for (auto& listener : listeners_) {
            listener->OnTick(delta);
        }
    }

}