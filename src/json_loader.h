#pragma once

#include <boost/json.hpp>
#include <filesystem>

#include "model.h"
#include "extra_data.h"

namespace json_loader {

using namespace boost::json;

value ParseJsonFromFile(const std::filesystem::path& json_path);

void LoadRoads(model::Map& map, const array& roads_json);

void LoadBuildings(model::Map& map, const array& buildings_json);

void LoadOffices(model::Map& map, const array& offices_json);

model::Game LoadGame(const std::filesystem::path& json_path, bool randomize_spawn_points, ExtraData& ex_data, double& dog_retirement_time);

}  // namespace json_loader
