#include <catch2/catch_test_macros.hpp>

#include "../src/model.h"
#include <memory>

using namespace model;
using namespace std::chrono_literals;

class MockLootGenerator : public loot_gen::LootGenerator {
    public:
        using Base = loot_gen::LootGenerator;
        MockLootGenerator() : Base(1000ms, 1.0) {}
        
        // Позволяет явно задать, сколько лута сгенерировать
        void SetNextLoot(unsigned count) {
            next_loot_ = count;
        }
    
        unsigned Generate(TimeInterval, unsigned, unsigned) override {
            return next_loot_;
        }
    
    private:
        unsigned next_loot_ = 0;
    };

    TEST_CASE("Game generates loot correctly using LootGenerator", "[Loots]") {
        auto loot_gen = std::make_shared<MockLootGenerator>();
    
        Game game{false, loot_gen};
    
        Map test_map(Map::Id{"map1"}, "TestMap", 10);
        test_map.AddRoad(Road{Road::HORIZONTAL, Point{0, 0}, 10});
        game.AddMap(test_map);
    
        auto session = game.GetSession(Map::Id{"map1"});
        session->AddDog("Doggy", 0);  // 1 мародер на карте
    
        SECTION("No loot generated when generator returns 0") {
            loot_gen->SetNextLoot(0);
            session->AddLoots(loot_gen->Generate(1000ms, 0, 1));
            REQUIRE(session->GetNumLoots() == 0);
        }
    
        SECTION("Loot is added to session when generator returns > 0") {
            loot_gen->SetNextLoot(3);
            session->AddLoots(loot_gen->Generate(1000ms, 0, 5));
            REQUIRE(session->GetNumLoots() == 3);
        }
    
        SECTION("Loot count does not exceed looter count") {
            session->AddDog("Dog2", 0); // теперь 2 мародера
            unsigned looters = 2;
            unsigned current_loot = session->GetNumLoots();
        
            // Воспроизводим реальную логику
            unsigned generated = loot_gen->Generate(1000ms, current_loot, looters);
            REQUIRE(generated <= looters); // тут должен отрабатывать LootGenerator
        
            session->AddLoots(generated);
            REQUIRE(session->GetNumLoots() == generated); // проверим, что добавлено ровно столько
        }
    }

