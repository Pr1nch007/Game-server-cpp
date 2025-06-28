#define _USE_MATH_DEFINES

#include "../src/collision_detector.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_contains.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <sstream>
#include <vector>
#include <cmath>
#include <set>

using Catch::Matchers::WithinAbs;

const double EPSILON = 1e-10;

namespace Catch {
template<>
struct StringMaker<collision_detector::GatheringEvent> {
  static std::string convert(collision_detector::GatheringEvent const& value) {
      std::ostringstream tmp;
      tmp << "(" << value.gatherer_id << "," << value.item_id << "," 
          << value.sq_distance << "," << value.time << ")";
      return tmp.str();
  }
};
}  // namespace Catch

class TestProvider : public collision_detector::ItemGathererProvider {
public:
    TestProvider(std::vector<collision_detector::Item> items, 
                 std::vector<collision_detector::Gatherer> gatherers)
        : items_(std::move(items)), gatherers_(std::move(gatherers)) {}

    size_t ItemsCount() const override {
        return items_.size();
    }

    collision_detector::Item GetItem(size_t idx) const override {
        return items_.at(idx);
    }

    size_t GatherersCount() const override {
        return gatherers_.size();
    }

    collision_detector::Gatherer GetGatherer(size_t idx) const override {
        return gatherers_.at(idx);
    }

private:
    std::vector<collision_detector::Item> items_;
    std::vector<collision_detector::Gatherer> gatherers_;
};

TEST_CASE("No items - no events", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items;
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {10, 0}, 1.0}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.empty());
}

TEST_CASE("No gatherers - no events", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{5, 0}, 0.5}
    };
    std::vector<collision_detector::Gatherer> gatherers;

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.empty());
}

TEST_CASE("Single gatherer collects single item", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{5, 0}, 0.5}
    };
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {10, 0}, 1.0}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.size() == 1);
    CHECK(events[0].item_id == 0);
    CHECK(events[0].gatherer_id == 0);
    CHECK_THAT(events[0].time, WithinAbs(0.5, EPSILON));
    CHECK_THAT(events[0].sq_distance, WithinAbs(0.0, EPSILON));
}

TEST_CASE("Gatherer misses item", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{5, 2.1}, 0.5}  
    };
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {10, 0}, 1.0}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.empty());
}

TEST_CASE("Gatherer collects multiple items", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{2, 0}, 0.5, 0},
        {{5, 0}, 0.5, 1},
        {{8, 0}, 0.5, 2}
    };
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {10, 0}, 1.0}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.size() == 3);
    
    CHECK(events[0].time <= events[1].time);
    CHECK(events[1].time <= events[2].time);
    
    std::vector<size_t> collected_items;
    for (const auto& event : events) {
        collected_items.push_back(event.item_id);
    }
    std::sort(collected_items.begin(), collected_items.end());
    CHECK(collected_items == std::vector<size_t>{0, 1, 2});
}

TEST_CASE("Multiple gatherers collect items", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{2, 0}, 0.5, 0},
        {{5, 5}, 0.5, 1},
        {{8, 0}, 0.5, 2}
    };
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {10, 0}, 1.0, 0},
        {{0, 0}, {10, 10}, 1.0, 1}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.size() == 4);
    
    std::set<size_t> collected_items;
    for (const auto& event : events) {
        collected_items.insert(event.item_id);
    }
    REQUIRE(collected_items.size() == 3);
}

TEST_CASE("Item can be collected by multiple gatherers", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{5, 0}, 0.5}
    };
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {10, 0}, 1.0, 0},
        {{0, 0}, {10, 0}, 1.0, 1}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.size() == 2);
    CHECK(events[0].item_id == 0);
    CHECK(events[1].item_id == 0);
    CHECK_THAT(events[0].time, WithinAbs(0.5, EPSILON));
    CHECK_THAT(events[1].time, WithinAbs(0.5, EPSILON));
    CHECK((events[0].gatherer_id == 0 || events[0].gatherer_id == 1));
    CHECK((events[1].gatherer_id == 0 || events[1].gatherer_id == 1));
    CHECK(events[0].gatherer_id != events[1].gatherer_id);
}

TEST_CASE("Stationary gatherer collects nothing", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{0, 0}, 0.5}
    };
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {0, 0}, 1.0}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.empty());
}

TEST_CASE("Item with width is collected correctly", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{5, 1.4}, 0.5}
    };
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {10, 0}, 1.0}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.size() == 1);
    CHECK(events[0].item_id == 0);
    CHECK(events[0].gatherer_id == 0);
    CHECK_THAT(events[0].time, WithinAbs(0.5, EPSILON));
    CHECK_THAT(events[0].sq_distance, WithinAbs(1.4 * 1.4, EPSILON));
}

TEST_CASE("Items are collected in correct order", "[FindGatherEvents]") {
    std::vector<collision_detector::Item> items{
        {{3, 0}, 0.5, 0},
        {{1, 0}, 0.5, 1},
        {{5, 0}, 0.5, 2}
    };
    std::vector<collision_detector::Gatherer> gatherers{
        {{0, 0}, {10, 0}, 1.0}
    };

    TestProvider provider(items, gatherers);
    auto events = collision_detector::FindGatherEvents(provider);

    REQUIRE(events.size() == 3);
    
    CHECK(events[0].time < events[1].time);
    CHECK(events[1].time < events[2].time);
    
    CHECK(events[0].item_id == 1);
    CHECK(events[1].item_id == 0);
    CHECK(events[2].item_id == 2);
}
    