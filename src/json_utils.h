#pragma once
#include <boost/json.hpp>
#include "model.h"

namespace json_utils {

using namespace boost::json;

object RoadToJson(const model::Road& road);
object BuildingToJson(const model::Building& building);
object OfficeToJson(const model::Office& office);
object MapToJson(const model::Map& map);

}