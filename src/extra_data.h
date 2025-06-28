#pragma once

#include <boost/json.hpp>
#include <string>

using namespace boost::json;

class ExtraData {
public:
    void SetLootsInMap (const std::string& map, value loot) {
        if (loots_in_map_.contains(map)) {
            array& loot_array = loots_in_map_[map].as_array();
            loot_array.push_back(std::move(loot));
        } else {
            loots_in_map_[map] = array{std::move(loot)};
        }
    }

    array GetLootsForMap(const std::string& map) const {
        if (!loots_in_map_.contains(map)) {
            return {};
        }
        return loots_in_map_.at(map).as_array();
    }

    int GetValueLoot(const std::string& map, int type) {
        if (!loots_in_map_.contains(map)) {
            throw "No map for value";
        }
        array& loot_array = loots_in_map_[map].as_array();
        return loot_array[type].at("value").as_int64();
    }

private:
    object loots_in_map_;
};