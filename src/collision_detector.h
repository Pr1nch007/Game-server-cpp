#pragma once

#include "geom.h"

#include <algorithm>
#include <vector>

namespace collision_detector {

struct CollectionResult {
    bool IsCollected(double collect_radius) const {
        return proj_ratio >= 0 && proj_ratio <= 1 && sq_distance <= collect_radius * collect_radius;
    }

    double sq_distance;

    double proj_ratio;
};


CollectionResult TryCollectPoint(geom::Point2D a, geom::Point2D b, geom::Point2D c);

struct Item {
    geom::Point2D position;
    double width;
    int id = 0;
};

struct Gatherer {
    geom::Point2D start_pos;
    geom::Point2D end_pos;
    double width;
    int id = 0;
};

class ItemGathererProvider {
protected:
    ~ItemGathererProvider() = default;

public:
    virtual size_t ItemsCount() const = 0;
    virtual Item GetItem(size_t idx) const = 0;
    virtual size_t GatherersCount() const = 0;
    virtual Gatherer GetGatherer(size_t idx) const = 0;
};

struct Provider : public ItemGathererProvider {
    const std::vector<Item>& items;
    const std::vector<Gatherer>& gatherers;

    Provider(const std::vector<Item>& i, const std::vector<Gatherer>& g)
        : items(i), gatherers(g) {}

    size_t ItemsCount() const override { return items.size(); }
    Item GetItem(size_t idx) const override { return items[idx]; }
    size_t GatherersCount() const override { return gatherers.size(); }
    Gatherer GetGatherer(size_t idx) const override { return gatherers[idx]; }
};

struct GatheringEvent {
    size_t item_id;
    size_t gatherer_id;
    double sq_distance;
    double time;
};

std::vector<GatheringEvent> FindGatherEvents(const ItemGathererProvider& provider);

}  // namespace collision_detector