#include "Death.h"
#include "data/Status.h"
#include "data/Location.h"
#include "plants/Flower.h"
#include <vector>
#include <random>
#include <cmath>

namespace simbio {
    namespace systems {
        using namespace organism;
        using namespace plants;

        void Death::registerDeathSystem(flecs::world& world, std::vector<std::vector<organism::SimpleEntity>>& chunkGrid)
        {
            world.system<Status, Location>("DeathSystem")
                .each([&](flecs::entity e, const Status& status, const Location& location)
                    {
                        if (status.energy < 0.0f || status.health < 0.0f) {
                            auto& bucket = chunkGrid[location.chunk];
                            for (int i = 0; i < bucket.size(); ++i) {
                                if (bucket[i].entityId == e.id()) {
                                    bucket[i] = bucket.back();
                                    bucket.pop_back();
                                    break;
                                }
                            }
                            e.destruct();
                        }
                    });
        }

        void Death::registerFlowerDeathSystem(flecs::world& world, std::vector<std::vector<organism::SimpleEntity>>& chunkGrid)
        {
		    static std::mt19937 rng{ std::random_device{}() };
            static std::exponential_distribution<float> flowerDeathTimerDist{ 1.0f / DEATH_TIMER_SPREAD };

            world.observer<Flower, const Location>("FlowerDeathTimerObserver")
                .event(flecs::OnSet)
                .each([this](flecs::entity e, Flower& flower, const Location& location) {
                    deathQueue[(deathTick + (int)std::ceil((MIN_DEATH_TIMER + flowerDeathTimerDist(rng)) / 0.1f)) % MAX_DEATH_TICKS]
                        .emplace_back(organism::SimpleEntity{ location, 0, e.id() });
                });

            world.system("FlowerDeathSystem")
                .run([this, &world, &chunkGrid](flecs::iter& it) {
                    auto& bucket = deathQueue[deathTick];
                    std::vector<SimpleEntity> flowers;
                    flowers.swap(bucket);
                    for (auto flower : flowers) {
                        auto& bucket = chunkGrid[flower.location.chunk];
                        for (int i = 0; i < bucket.size(); ++i) {
                            if (bucket[i].entityId == flower.entityId) {
                                bucket[i] = bucket.back();
                                bucket.pop_back();
                                break;
                            }
                        }
                        world.entity(flower.entityId).destruct();
                    }
                    deathTick = (deathTick + 1) % MAX_DEATH_TICKS;
                });
        }
    }
}