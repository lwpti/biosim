#include "Death.h"
#include "data/Status.h"
#include "data/Location.h"
#include "organs/Flower.h"
#include <vector>
#include <random>
#include <cmath>
#include "Entity.h"

namespace simbio {
    namespace systems {
        using namespace organism;

        void Death::registerDeathSystem(flecs::world& world, std::vector<std::vector<organism::Entity>>& chunkGrid)
        {
            world.system<Status, Location>("DeathSystem")
                .each([&](flecs::entity e, const Status& status, const Location& location)
                    {
                        if (status.energy < 0.0f || status.health < 0.0f) {
                            auto& bucket = chunkGrid[location.chunk];
                            for (int i = 0; i < bucket.size(); ++i) {
                                if (bucket[i].flecsID == e.id()) {
                                    bucket[i] = bucket.back();
                                    bucket.pop_back();
                                    break;
                                }
                            }
                            e.destruct();
                        }
                    });
        }

        void Death::registerFlowerDeathSystem(flecs::world& world, std::vector<std::vector<organism::Entity>>& chunkGrid)
        {
		    static std::mt19937 rng{ std::random_device{}() };
            static std::exponential_distribution<float> flowerDeathTimerDist{ 1.0f / DEATH_TIMER_SPREAD };

            world.observer<Flower, const Location>("FlowerDeathTimerObserver")
                .event(flecs::OnSet)
                .each([this](flecs::entity e, Flower& flower, const Location& location) {
                    deathQueue[(deathTick + (int)std::ceil((MIN_DEATH_TIMER + flowerDeathTimerDist(rng)) / 0.1f)) % MAX_DEATH_TICKS]
                        .emplace_back(organism::Entity{ .flecsID = e.id(), .location = location });
                });

            world.system("FlowerDeathSystem")
                .run([this, &world, &chunkGrid](flecs::iter& it) {
                    auto& bucket = deathQueue[deathTick];
                    std::vector<Entity> flowers;
                    flowers.swap(bucket);
                    for (auto flower : flowers) {
                        auto& bucket = chunkGrid[flower.location.chunk];
                        for (int i = 0; i < bucket.size(); ++i) {
                            if (bucket[i].flecsID == flower.flecsID) {
                                bucket[i] = bucket.back();
                                bucket.pop_back();
                                break;
                            }
                        }
                        world.entity(flower.flecsID).destruct();
                    }
                    deathTick = (deathTick + 1) % MAX_DEATH_TICKS;
                });
        }
    }
}