#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <random>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <optional>
#include <iostream>
#include <limits>
#include <chrono>

#include "tagged.h"
#include "loot_generator.h"
#include "collision_detector.h"

namespace model {

using Dimension = int;
using Coord = Dimension;

struct Point {
    Coord x, y;
};

struct Size {
    Dimension width, height;
};

struct Rectangle {
    Point position;
    Size size;
};

struct Offset {
    Dimension dx, dy;
};

struct Position {
    double x, y;

    bool operator==(const Position& other) const noexcept;
};

struct PositionHasher {
    std::size_t operator()(const Position& position) const noexcept;
};

struct Velocity {
    double dx = 0.0;
    double dy = 0.0;

    bool IsZero() {
        return dx == 0 && dy == 0;
    }
};

enum class Direction {
    NORTH,
    SOUTH,
    WEST,
    EAST
};

std::string DirectionToString(Direction dir);

std::optional<Direction> StringToDirection(const std::string& str);

class Road {
    struct HorizontalTag {
        HorizontalTag() = default;
    };

    struct VerticalTag {
        VerticalTag() = default;
    };

public:
    constexpr static HorizontalTag HORIZONTAL{};
    constexpr static VerticalTag VERTICAL{};

    Road(HorizontalTag, Point start, Coord end_x) noexcept
        : start_{start}
        , end_{end_x, start.y} {
    }

    Road(VerticalTag, Point start, Coord end_y) noexcept
        : start_{start}
        , end_{start.x, end_y} {
    }

    bool IsHorizontal() const noexcept {
        return start_.y == end_.y;
    }

    bool IsVertical() const noexcept {
        return start_.x == end_.x;
    }

    Point GetStart() const noexcept {
        return start_;
    }

    Point GetEnd() const noexcept {
        return end_;
    }

private:
    Point start_;
    Point end_;
};

class Building {
public:
    explicit Building(Rectangle bounds) noexcept
        : bounds_{bounds} {
    }

    const Rectangle& GetBounds() const noexcept {
        return bounds_;
    }

private:
    Rectangle bounds_;
};

class Office {
public:
    using Id = util::Tagged<std::string, Office>;

    Office(Id id, Point position, Offset offset) noexcept
        : id_{std::move(id)}
        , position_{position}
        , offset_{offset} {
    }

    const Id& GetId() const noexcept {
        return id_;
    }

    Point GetPosition() const noexcept {
        return position_;
    }

    Offset GetOffset() const noexcept {
        return offset_;
    }

private:
    Id id_;
    Point position_;
    Offset offset_;
};

class Map {
public:
    using Id = util::Tagged<std::string, Map>;
    using Roads = std::vector<Road>;
    using Buildings = std::vector<Building>;
    using Offices = std::vector<Office>;
    using RoadLookup = std::unordered_map<Position, std::vector<std::shared_ptr<Road>>, PositionHasher>;

    Map(Id id, std::string name, int num_loots) noexcept
        : id_(std::move(id))
        , name_(std::move(name))
        , num_loots_(num_loots) {
    }

    const Id& GetId() const noexcept {
        return id_;
    }

    const std::string& GetName() const noexcept {
        return name_;
    }

    const Buildings& GetBuildings() const noexcept {
        return buildings_;
    }

    const Roads& GetRoads() const noexcept {
        return roads_;
    }

    const Offices& GetOffices() const noexcept {
        return offices_;
    }

    const std::optional<double> GetDogSpeed() const noexcept {
        return dog_speed_;
    }

    const std::optional<int> GetBagCapacity() const noexcept {
        return bag_capacity_;
    }

    void AddRoad(const Road& road) {
        roads_.emplace_back(road);
    }

    void AddBuilding(const Building& building) {
        buildings_.emplace_back(building);
    }

    void AddOffice(Office office);

    void AdddDogSpeed(double speed) {
        dog_speed_ = speed;
    }

    void AddBagCapacity(int capacity) {
        bag_capacity_ = capacity;
    }

    void BuildRoadLookup();

    bool IsOnRoad(const Position& pos);

    int GetNumLoots() const { return num_loots_; }

private:
    using OfficeIdToIndex = std::unordered_map<Office::Id, size_t, util::TaggedHasher<Office::Id>>;

    std::vector<std::shared_ptr<Road>> GetRoadsAt(Position pos) const;

    Id id_;
    std::string name_;
    Roads roads_;
    Buildings buildings_;

    OfficeIdToIndex warehouse_id_to_index_;
    Offices offices_;

    std::optional<double> dog_speed_;
    std::optional<int> bag_capacity_;

    RoadLookup road_lookup_;

    int num_loots_;
};

class Bag {
    public:
        explicit Bag(int capacity) : capacity_(capacity) {}
    
        void AddItem(int id, int type) {
            if (items_.size() >= capacity_) {
                throw "Bag full";
            }
            items_[id] = type;
        }

        void Clear() {
            items_.clear();
        }
        bool IsFull() const {
            return items_.size() >= capacity_;
        }
        int GetCapacity() const {
            return capacity_;
        }
        std::unordered_map<int, int> GetItems() const {
            return items_;
        }
    
    private:
        int capacity_;
        std::unordered_map<int, int> items_;
    };

class Dog {
    public:
        Dog(std::string name, uint32_t id, int bag_capacity, Position position, Velocity velocity = {0.0, 0.0}, Direction direction = Direction::NORTH)
        : name_(std::move(name)), id_(id), bag_(bag_capacity), position_(position), velocity_(velocity), direction_(direction),
        join_time_ms_(0), inactive_time_ms_(0) {}
    
        const std::string& GetName() const { return name_; }
        uint32_t GetId() const { return id_; }

        Bag& GetBag() { return bag_; }
        const Bag& ConstGetBag() const { return bag_; }

        Position GetPosition() const { return position_; }
        Velocity GetVelocity() const { return velocity_; }
        Direction GetDirection() const { return direction_; }

        void SetPosition(Position pos) { position_ = pos; }
        void SetVelocity(Velocity vel) { velocity_ = vel; }
        void SetDirection(Direction dir) { direction_ = dir; }

        void AddPoints(int points) { points_ += points; }
        int GetPoints() const { return points_; }

        void UpdateTimes(int64_t delta_ms) {
            join_time_ms_ += delta_ms;
            inactive_time_ms_ = 0;
        }

        void AddInactiveTime (int64_t delta_ms) { 
            join_time_ms_ += delta_ms;
            inactive_time_ms_+= delta_ms;
         }

        int64_t GetJoinTimeMs() const { return join_time_ms_; }
        int64_t GetInactiveTimeMs() const { return inactive_time_ms_; }
    
    private:
        std::string name_;
        uint32_t id_;
        Bag bag_;
        Position position_;
        Velocity velocity_;
        Direction direction_;
        int points_ = 0;
        int64_t join_time_ms_;    
        int64_t inactive_time_ms_;
    };

    class Loot {
    public:
        Loot(int type, Position pos, int id) : type_(type), pos_(pos), id_(id) {}

        int GetType() const { return type_; }
        Position GetPosition() const { return pos_; }
        int GetId() const { return id_; }
    private:
        int type_;
        Position pos_;
        int id_;
    };
    
    class GameSession {
    public:
        explicit GameSession(std::shared_ptr<model::Map> map, bool randomize_spawn_points) : map_(std::move(map)), randomize_spawn_points_(randomize_spawn_points) {}
    
        std::shared_ptr<Dog> AddDog(const std::string& name, int bag_capacity);
        void AddLoots (int num);
    
        const std::shared_ptr<Map> GetMap() const { return map_; }
        const std::unordered_map<uint32_t, std::shared_ptr<Dog>>& GetDogs() const { return dogs_; }
        const size_t GetNumLoots() const { return loots_.size(); }
        const std::unordered_map<int, std::shared_ptr<Loot>>& GetLoots() const {return loots_; }

        const std::shared_ptr<Dog> GetDog(uint32_t id) const;

        void RemoveLoot(int id) { loots_.erase(id); }
        void RemoveDog(uint32_t id) { dogs_.erase(id); }

        uint32_t GetNextIdDog() const { return next_id_dog_; }
        int GetNextIdLoot() const { return next_id_loot_; }

        void SetReadyDogs(std::unordered_map<uint32_t, std::shared_ptr<Dog>>&& dogs) { dogs_ = std::move(dogs);}
        void SetReadyLoots(std::unordered_map<int, std::shared_ptr<Loot>>&& loots) { loots_ = std::move(loots); }
        void SetNextIdDog(uint32_t next_id_dog) { next_id_dog_ = next_id_dog; }
        void SetNextIdLoot(int next_id_loot) { next_id_loot_ = next_id_loot; }


    
    private:
        Position GetRandomPositionOnRoad();
        
        std::shared_ptr<model::Map>  map_;
        std::unordered_map<uint32_t, std::shared_ptr<Dog>> dogs_;
        std::unordered_map<int, std::shared_ptr<Loot>> loots_;
        uint32_t next_id_dog_ = 0;
        int next_id_loot_ = 0;
        bool randomize_spawn_points_;
    };

class Game {
public:
    using Maps = std::vector<Map>;
    using MapIdHasher = util::TaggedHasher<Map::Id>;
    using MapIdToIndex = std::unordered_map<Map::Id, size_t, MapIdHasher>;
    using GameSessions = std::unordered_map<Map::Id, std::shared_ptr<GameSession>, MapIdHasher>;

    explicit Game(bool randomize_spawn_points, std::shared_ptr<loot_gen::LootGenerator> loot_gener) : randomize_spawn_points_(randomize_spawn_points), loot_gener_(loot_gener) {}

    void AddMap(Map map);

    void AddDefaultDogSpeed (double speed) {
        default_dog_speed_ = speed;
    }

    void AddDefaultBagCapacity (int capacity) {
        default_bag_capacity_ = capacity;
    }

    const Maps& GetMaps() const noexcept {
        return maps_;
    }

    const Map* FindMap(const Map::Id& id) const noexcept;

    std::shared_ptr<GameSession> GetSession(const Map::Id& id);

    double GetDefaultDogSpeed(const Map::Id& id) const noexcept;
    int GetDefaultBagCapacity(const Map::Id& id) const noexcept;

    GameSessions GetSessions() { return sessions_; }

    std::shared_ptr<loot_gen::LootGenerator> GetLootGenerator() { return loot_gener_; }
    
    bool GetSpawnPoints() const { return randomize_spawn_points_; }

    void SetGameSessions(GameSessions sessions) { sessions_ = sessions; }

private:
    std::vector<Map> maps_;
    MapIdToIndex map_id_to_index_;
    GameSessions sessions_;
    double default_dog_speed_ = 1.0;
    bool randomize_spawn_points_;
    std::shared_ptr<loot_gen::LootGenerator> loot_gener_;
    int default_bag_capacity_ = 3;
};

}  // namespace model   


