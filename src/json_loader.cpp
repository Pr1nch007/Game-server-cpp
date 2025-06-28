#include "json_loader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <cstdint>

#include "loot_generator.h"

namespace json_loader {

constexpr std::string_view kX = "x";
constexpr std::string_view kY = "y";

value ParseJsonFromFile(const std::filesystem::path& json_path) {
    std::ifstream file(json_path);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл: " + json_path.string());
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return parse(buffer.str());
}

void LoadRoads(model::Map& map, const array& roads_json) {
    for (const auto& road_json : roads_json) {
        int x0 = road_json.at("x0").as_int64();
        int y0 = road_json.at("y0").as_int64();

        if (road_json.as_object().contains("x1")) {
            int x1 = road_json.at("x1").as_int64();
            map.AddRoad(model::Road(model::Road::HORIZONTAL, {x0, y0}, x1));
        } else if (road_json.as_object().contains("y1")) {
            int y1 = road_json.at("y1").as_int64();
            map.AddRoad(model::Road(model::Road::VERTICAL, {x0, y0}, y1));
        }
    }
}

void LoadBuildings(model::Map& map, const array& buildings_json) {
    for (const auto& building_json : buildings_json) {
        int x = building_json.at(kX.data()).as_int64();
        int y = building_json.at(kY.data()).as_int64();
        int w = building_json.at("w").as_int64();
        int h = building_json.at("h").as_int64();

        map.AddBuilding(model::Building({{x, y}, {w, h}}));
    }
}

void LoadOffices(model::Map& map, const array& offices_json) {
    for (const auto& office_json : offices_json) {
        std::string office_id = office_json.at("id").as_string().c_str();
        int x = office_json.at(kX.data()).as_int64();
        int y = office_json.at(kY.data()).as_int64();
        int offsetX = office_json.at("offsetX").as_int64();
        int offsetY = office_json.at("offsetY").as_int64();

        map.AddOffice(model::Office(model::Office::Id{office_id}, {x, y}, {offsetX, offsetY}));
    }
}

model::Game LoadGame(const std::filesystem::path& json_path, bool randomize_spawn_points, ExtraData& ex_data, double& dog_retirement_time) {
    value parsed_json = ParseJsonFromFile(json_path);
    
    const auto& loot_generator_config = parsed_json.as_object().at("lootGeneratorConfig").as_object();
    std::uint64_t  milliseconds_value = static_cast<uint64_t>(loot_generator_config.at("period").as_double() * 1000);
    std::shared_ptr<loot_gen::LootGenerator> loot_gen = std::make_shared<loot_gen::LootGenerator>(std::chrono::milliseconds(milliseconds_value), 
                                                                                                loot_generator_config.at("probability").as_double());

    model::Game game (randomize_spawn_points, loot_gen);

    if(parsed_json.as_object().count("defaultDogSpeed")) {
        double speed = parsed_json.as_object().at("defaultDogSpeed").as_double();
        game.AddDefaultDogSpeed(speed);
    }

    if(parsed_json.as_object().count("defaultBagCapacity")) {
        int capacity = parsed_json.as_object().at("defaultBagCapacity").as_int64();
        game.AddDefaultBagCapacity(capacity);
    }

    if (parsed_json.as_object().count("dogRetirementTime")) {
        dog_retirement_time = parsed_json.as_object().at("dogRetirementTime").as_double();
    } else {
        dog_retirement_time = 60.0;
    }
    
    const auto& maps = parsed_json.as_object().at("maps").as_array();
    for (const auto& map_json : maps) {
        std::string id = map_json.at("id").as_string().c_str();
        std::string name = map_json.at("name").as_string().c_str();

        const array& loot_types = map_json.at("lootTypes").as_array();
        for (const value& loot : loot_types) {
            ex_data.SetLootsInMap(id, loot);
        }
        int num_loots = static_cast<int>(loot_types.size());

        model::Map map(model::Map::Id{id}, name, num_loots);

        LoadRoads(map, map_json.at("roads").as_array());
        LoadBuildings(map, map_json.at("buildings").as_array());
        LoadOffices(map, map_json.at("offices").as_array());

        if(map_json.as_object().count("dogSpeed")) {
            double speed = map_json.at("dogSpeed").as_double();
            map.AdddDogSpeed(speed);
        }

        if(map_json.as_object().count("bagCapacity")) {
            int capacity = map_json.at("bagCapacity").as_int64();
            map.AddBagCapacity(capacity);
        }

        map.BuildRoadLookup();

        game.AddMap(std::move(map));
    }



    return game;
}

}  // namespace json_loader
