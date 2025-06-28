#include "model.h"

#include <stdexcept>
#include <cstdlib>
#include <ctime>

namespace model {
using namespace std::literals;

bool Position::operator==(const Position& other) const noexcept {
    const double epsilon = 1e-5;
    return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon;
}

std::size_t PositionHasher::operator()(const Position& position) const noexcept {
    auto round_x = static_cast<int>(position.x * 1000);
    auto round_y = static_cast<int>(position.y * 1000);

    return std::hash<int>{}(round_x) ^ (std::hash<int>{}(round_y) << 1);
}

void Map::AddOffice(Office office) {
    if (warehouse_id_to_index_.contains(office.GetId())) {
        throw std::invalid_argument("Duplicate warehouse");
    }

    const size_t index = offices_.size();
    Office& o = offices_.emplace_back(std::move(office));
    try {
        warehouse_id_to_index_.emplace(o.GetId(), index);
    } catch (...) {
        // Удаляем офис из вектора, если не удалось вставить в unordered_map
        offices_.pop_back();
        throw;
    }
}

void Game::AddMap(Map map) {
    const size_t index = maps_.size();
    if (auto [it, inserted] = map_id_to_index_.emplace(map.GetId(), index); !inserted) {
        throw std::invalid_argument("Map with id "s + *map.GetId() + " already exists"s);
    } else {
        try {
            maps_.emplace_back(std::move(map));
        } catch (...) {
            map_id_to_index_.erase(it);
            throw;
        }
    }
}

std::string DirectionToString(Direction dir) {
    switch (dir) {
        case Direction::WEST: return "L";
        case Direction::EAST: return "R";
        case Direction::NORTH: return "U";
        case Direction::SOUTH: return "D";
    }
    return "";
}

std::optional<Direction> StringToDirection(const std::string& str) {
    if(str.empty()){
        return {};
    } else if (str == "L") {
        return Direction::WEST;
    } else if (str == "R") {
        return Direction::EAST;
    } else if (str == "U") {
        return Direction::NORTH;
    } else if (str == "D") {
        return Direction::SOUTH;
    }
    throw std::invalid_argument("Invalid direction string");
}

void Map::BuildRoadLookup() {
    for (const auto& road : roads_) {
        if (road.IsHorizontal()) {
            for (double x = road.GetStart().x; x <= road.GetEnd().x; x += 0.5) {
                road_lookup_[{x, static_cast<double>(road.GetStart().y)}].push_back(std::make_shared<Road>(road));
            }
        } else if (road.IsVertical()) {
            for (double y = road.GetStart().y; y <= road.GetEnd().y; y += 0.5) {
                road_lookup_[{static_cast<double>(road.GetStart().x), y}].push_back(std::make_shared<Road>(road));
            }
        }
    }
}

bool Map::IsOnRoad(const Position& pos) {
    constexpr double WIDTH_ROAD = 0.4;
    const double epsilon = std::numeric_limits<double>::epsilon();

    for (const auto& road : roads_) {
        if (road.IsHorizontal()) {
            const double road_y = static_cast<double>(road.GetStart().y);
            if (std::abs(pos.y - road_y) > WIDTH_ROAD + epsilon) continue;

            const double road_x1 = static_cast<double>(road.GetStart().x);
            const double road_x2 = static_cast<double>(road.GetEnd().x);
            const double min_x = std::min(road_x1, road_x2) - WIDTH_ROAD - epsilon;
            const double max_x = std::max(road_x1, road_x2) + WIDTH_ROAD + epsilon;

            if (pos.x >= min_x - epsilon && pos.x <= max_x + epsilon) {
                return true;
            }
        } else {
            const double road_x = static_cast<double>(road.GetStart().x);
            if (std::abs(pos.x - road_x) > WIDTH_ROAD + epsilon) continue;

            const double road_y1 = static_cast<double>(road.GetStart().y);
            const double road_y2 = static_cast<double>(road.GetEnd().y);
            const double min_y = std::min(road_y1, road_y2) - WIDTH_ROAD - epsilon;
            const double max_y = std::max(road_y1, road_y2) + WIDTH_ROAD + epsilon;

            if (pos.y >= min_y - epsilon && pos.y <= max_y + epsilon) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::shared_ptr<Road>> Map::GetRoadsAt(Position pos) const {
    if (auto it = road_lookup_.find(pos); it != road_lookup_.end()) {
        return it->second;
    }
    return {};
}

std::shared_ptr<Dog> GameSession::AddDog(const std::string& name, int bag_capacity) {
    Position position;
    if(randomize_spawn_points_) {
        position = GetRandomPositionOnRoad();
    } else {
        auto point = map_->GetRoads()[0].GetStart();
        position = {static_cast<double>(point.x), static_cast<double>(point.y)};
    }

    auto new_dog = std::make_shared<Dog>(name, next_id_dog_, bag_capacity, position);
    if (!new_dog) {
        std::cerr << "Error: failed to create a dog!" << std::endl;
        return nullptr;
    }
    dogs_[next_id_dog_++] = new_dog;
    return new_dog;
}

void GameSession::AddLoots (int num) {
    srand(time(NULL));
    int maxNumber = map_->GetNumLoots();

    for(int i = 0; i < num; ++i) {
        int randomNumber = rand() % maxNumber;
        Position pos = GetRandomPositionOnRoad();
    
        auto loot = std::make_shared<Loot>(randomNumber, pos, next_id_loot_);

        loots_[next_id_loot_++] = loot;
    }

}

Position GameSession::GetRandomPositionOnRoad() {
    if (map_->GetRoads().empty()) {
        return {0.0, 0.0};
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    const auto& roads = map_->GetRoads();
    std::uniform_int_distribution<size_t> road_dist(0, roads.size() - 1);
    const auto& road = roads[road_dist(gen)];

    if (road.IsHorizontal()) {
        std::uniform_real_distribution<double> x_dist(road.GetStart().x, road.GetEnd().x);
        return {x_dist(gen), static_cast<double>(road.GetStart().y)};
    } else {
        std::uniform_real_distribution<double> y_dist(road.GetStart().y, road.GetEnd().y);
        return {static_cast<double>(road.GetStart().x), y_dist(gen)};
    }
}

const std::shared_ptr<Dog> GameSession::GetDog(uint32_t id) const {
    auto it = dogs_.find(id);
    if (it != dogs_.end()) {
        return it->second;
    }
    return nullptr;
}

const Map* Game::FindMap(const Map::Id& id) const noexcept {
    if (auto it = map_id_to_index_.find(id); it != map_id_to_index_.end()) {
        return &maps_.at(it->second);
    }
    return nullptr;
}

std::shared_ptr<GameSession> Game::GetSession(const Map::Id& id) {
    auto it = sessions_.find(id);
    if (it != sessions_.end()) {
        return it->second;
    }
    std::shared_ptr<model::Map> mapPtr = std::make_shared<model::Map>(*FindMap(id));
    auto session = std::make_shared<GameSession>(mapPtr, randomize_spawn_points_);
    sessions_[id] = session;
    return session;
}

double Game::GetDefaultDogSpeed(const Map::Id& id) const noexcept {
    auto map = FindMap(id);
    auto speed = map->GetDogSpeed();
    if (speed) {
        return speed.value();
    }
    return default_dog_speed_;
}

int Game::GetDefaultBagCapacity(const Map::Id& id) const noexcept {
    auto map = FindMap(id);
    auto capacity = map->GetBagCapacity();
    if (capacity) {
        return capacity.value();
    }
    return default_bag_capacity_;
}

}  // namespace model
