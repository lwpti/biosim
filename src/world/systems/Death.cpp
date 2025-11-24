#include "Death.h"
#include <data/Status.h>
#include <data/Location.h>
#include <plants/Flower.h>
#include <vector>
#include <random>
#include <cmath>

namespace simbio {
    namespace systems {
        using namespace organism;
        using namespace plants;

        void Death::registerDeathSystem(flecs::world& world, std::vector<std::vector<flecs::entity_t>>& chunkGrid)
        {
            world.system<Status, Location>("DeathSystem")
                .each([&](flecs::entity e, const Status& status, const Location& location)
                    {
                        if (status.energy < 0.0f || status.health < 0.0f) {
                            auto& bucket = chunkGrid[location.chunk];
                            for (int i = 0; i < bucket.size(); ++i) {
                                if (bucket[i] == e.id()) {
                                    bucket[i] = bucket.back();
                                    bucket.pop_back();
                                    break;
                                }
                            }
                            e.destruct();
                        }
                    });
        }

        void Death::registerFlowerDeathSystem(flecs::world& world, std::vector<std::vector<flecs::entity_t>>& chunkGrid)
        {
            world.system<Flower, const Location>("FlowerDeathSystem")
                .each([&](flecs::entity e, Flower& flower, const Location& location)
                    {
                        if (flower.deathTimer > 0) {
                            --flower.deathTimer;
                        }
                        else if (flower.deathTimer == 0) {
                            auto& bucket = chunkGrid[location.chunk];
                            for (int i = 0; i < bucket.size(); ++i) {
                                if (bucket[i] == e.id()) {
                                    bucket[i] = bucket.back();
                                    bucket.pop_back();
                                    break;
                                }
                            }
                            e.destruct();
                        }
                    });
        }
    }
}
