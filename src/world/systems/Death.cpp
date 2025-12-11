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

        void Death::registerDeathSystem(World& world) {
            world.flecsWorld.system<Status, Location>().each(
                [&](flecs::entity e, const Status& status, const Location& location) {
                    if (status.energy <= 0.0f || status.health <= 0.0f) {
                        auto& bucket = world.chunkGrid[location.chunk];
                        for (int i = 0; i < bucket.size(); ++i) {
                            if (bucket[i].flecsID == e.id()) {
                                bucket[i] = bucket.back();
                                bucket.pop_back();
                                break;
                            }
                        }
                        e.destruct();
                    }
                }
            );
        }

        void Death::registerFlowerDeathSystem(World& world) {
		    static std::mt19937 rng{ std::random_device{}() };
            static std::exponential_distribution<float> flowerDeathTimerDist{ 1.0f / DEATH_TIMER_SPREAD };

            world.flecsWorld.observer<Flower, const Location>().event(flecs::OnSet).each(
                [&](flecs::entity entity, Flower& flower, const Location& location) {
                deathQueue[(deathTick + (int)std::ceil((MIN_DEATH_TIMER + flowerDeathTimerDist(rng)) / world.timeStep)) % MAX_DEATH_TICKS].emplace_back(
                    Entity{ .flecsID = entity.id(), .color = Flower::FLOWER_COLOR, .location = entity.get<Location>(), .organs = { .flower = flower } });
            });

            world.flecsWorld.system().kind(flecs::OnUpdate).run(
                [&](flecs::iter& it) {
                auto& bucket = deathQueue[deathTick];
                std::vector<Entity> flowers;
                flowers.swap(bucket);
                for (auto flower : flowers) {
                    auto& bucket = world.chunkGrid[flower.location.chunk];
                    for (int i = 0; i < bucket.size(); ++i) {
                        if (bucket[i].flecsID == flower.flecsID) {
                            bucket[i] = bucket.back();
                            bucket.pop_back();
                            break;
                        }
                    }
                    flecs::entity entity = world.flecsWorld.entity(flower.flecsID);
                    if (entity.is_alive()) entity.destruct();
                }
                deathTick = (deathTick + 1) % MAX_DEATH_TICKS;
            });
        }
    }
}