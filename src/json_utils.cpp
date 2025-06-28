#include "json_utils.h"

namespace json_utils {

constexpr std::string_view kX = "x";
constexpr std::string_view kY = "y";

object RoadToJson(const model::Road& road) {
    object road_json;
    road_json["x0"] = road.GetStart().x;
    road_json["y0"] = road.GetStart().y;

    if (road.IsHorizontal()) {
        road_json["x1"] = road.GetEnd().x;
    } else if (road.IsVertical()) {
        road_json["y1"] = road.GetEnd().y;
    }
    return road_json;
}

object BuildingToJson(const model::Building& building) {
    object building_json;
    const auto& bounds = building.GetBounds();
    building_json[kX.data()] = bounds.position.x;
    building_json[kY.data()] = bounds.position.y;
    building_json["w"] = bounds.size.width;
    building_json["h"] = bounds.size.height;
    return building_json;
}

object OfficeToJson(const model::Office& office) {
    object office_json;
    office_json["id"] = *office.GetId();
    office_json[kX.data()] = office.GetPosition().x;
    office_json[kY.data()] = office.GetPosition().y;
    office_json["offsetX"] = office.GetOffset().dx;
    office_json["offsetY"] = office.GetOffset().dy;
    return office_json;
}

object MapToJson(const model::Map& map) {
    object map_json;
    map_json["id"] = *map.GetId();
    map_json["name"] = map.GetName();

    array roads_json;
    for (const auto& road : map.GetRoads()) {
        roads_json.push_back(RoadToJson(road));
    }
    map_json["roads"] = std::move(roads_json);

    array buildings_json;
    for (const auto& building : map.GetBuildings()) {
        buildings_json.push_back(BuildingToJson(building));
    }
    map_json["buildings"] = std::move(buildings_json);

    array offices_json;
    for (const auto& office : map.GetOffices()) {
        offices_json.push_back(OfficeToJson(office));
    }
    map_json["offices"] = std::move(offices_json);

    return map_json;
}

}