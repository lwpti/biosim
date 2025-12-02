#include "Eating.h"

#include <cmath>
#include <algorithm>

#include "data/Location.h"
#include "data/Status.h"
#include "organs/Flower.h"
#include "organs/Arms.h"
#include "organs/Mouth.h"
#include "percepts/Touch.h"

namespace simbio {
    namespace systems {
        using namespace organism;

        // Helper to scan neighboring chunks
        template <typename Func>
        static void forNeighborChunks(
            int chunkX, int chunkY,
            int chunkCols, int chunkRows,
            Func&& func)
        {
            for (int dy = -1; dy <= 1; ++dy) {
                int adjY = chunkY + dy;
                if (adjY < 0) adjY = chunkRows - 1;
                if (adjY >= chunkRows) adjY = 0;

                for (int dx = -1; dx <= 1; ++dx) {
                    int adjX = chunkX + dx;
                    if (adjX < 0) adjX = chunkCols - 1;
                    if (adjX >= chunkCols) adjX = 0;

                    func(adjX, adjY);
                }
            }
        }

        void Eating::registerGrabbingSystem(
            flecs::world& world,
            std::vector<std::vector<Entity>>& chunkGrid,
            int chunkSize,
            int chunkCols,
            int chunkRows)
        {
            world.system<Arms, ArmsIntent, Location>("GrabbingSystem")
                .each([&, chunkSize, chunkCols, chunkRows]
                (flecs::entity grabber, Arms& arms, ArmsIntent /*unused*/, Location& loc)
                    {
                        float grabRange = arms.size + 5.0f;
                        float bestDist2 = grabRange * grabRange;
                        flecs::entity bestTarget = flecs::entity::null();

                        int chunkX = (int)(loc.x / chunkSize);
                        int chunkY = (int)(loc.y / chunkSize);

                        forNeighborChunks(chunkX, chunkY, chunkCols, chunkRows,
                            [&](int adjX, int adjY)
                            {
                                auto& bucket = chunkGrid[adjY * chunkCols + adjX];
                                for (const auto& candidate : bucket) {
                                    if (candidate.flecsID == grabber.id()) continue;

                                    float dx = candidate.location.x - loc.x;
                                    float dy = candidate.location.y - loc.y;
                                    float dist2 = dx * dx + dy * dy;

                                    if (dist2 < bestDist2) {
                                        bestDist2 = dist2;
                                        bestTarget = flecs::entity(world, candidate.flecsID);
                                    }
                                }
                            });

                        if (bestTarget.is_valid() && bestTarget.is_alive()) {
                            grabber.set<Holding>({ bestTarget.id() });
                            bestTarget.set<Grabbed>({ grabber.id() });
                        }

                        // Clear the grab intent after processing
                        grabber.remove<ArmsIntent>();
                    });

        }

        void Eating::registerBitingSystem(
            flecs::world& world,
            std::vector<std::vector<Entity>>& chunkGrid,
            int chunkSize,
            int chunkCols,
            int chunkRows)
        {
            world.system<Mouth, BiteIntent, Location, Status>("BitingSystem")
                .each([&, chunkSize, chunkCols, chunkRows]
                (flecs::entity attacker, Mouth& mouth, BiteIntent /*unused*/,
                    Location& loc, Status& status)
                    {
                        float biteRange = mouth.size + 3.0f;
                        float bestDist2 = biteRange * biteRange;
                        flecs::entity bestTarget = flecs::entity::null();
                        int bestChunkIndex = -1;

                        int chunkX = (int)(loc.x / chunkSize);
                        int chunkY = (int)(loc.y / chunkSize);

                        forNeighborChunks(chunkX, chunkY, chunkCols, chunkRows,
                            [&](int adjX, int adjY)
                            {
                                int idx = adjY * chunkCols + adjX;
                                auto& bucket = chunkGrid[idx];
                                for (int i = 0; i < (int)bucket.size(); ++i) {
                                    const auto& candidate = bucket[i];
                                    if (candidate.flecsID == attacker.id()) continue;

                                    float dx = candidate.location.x - loc.x;
                                    float dy = candidate.location.y - loc.y;
                                    float dist2 = dx * dx + dy * dy;

                                    if (dist2 < bestDist2) {
                                        bestDist2 = dist2;
                                        bestTarget = flecs::entity(world, candidate.flecsID);
                                        bestChunkIndex = idx;
                                    }
                                }
                            });

                        if (!bestTarget.is_valid() || !bestTarget.is_alive()) {
                            attacker.remove<BiteIntent>();
                            return;
                        }

                        // Try to eat plants first
                        if (bestTarget.has<Flower>()) {
                            status.energy += 5.0f;

                            if (bestChunkIndex >= 0) {
                                auto& bucket = chunkGrid[bestChunkIndex];
                                for (int i = 0; i < (int)bucket.size(); ++i) {
                                    if (bucket[i].flecsID == bestTarget.id()) {
                                        bucket[i] = bucket.back();
                                        bucket.pop_back();
                                        break;
                                    }
                                }
                            }

                            bestTarget.destruct();
                        }
                        else {
                            // Damage other organisms (if they have Status)
                            if (bestTarget.has<Status>()) {
                                auto& victimStatus = bestTarget.get_mut<Status>();

                                float damage = mouth.size * 2.0f;
                                victimStatus.health -= damage;
                                status.energy += damage * 0.1f;
                            }
                        }

                        attacker.remove<BiteIntent>();
                    });

        }

    } // namespace systems
} // namespace simbio
