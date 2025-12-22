#include "FlowerReproduction.h"

#include <algorithm>
#include <cmath>
#include "data/Location.h"
#include "Entity.h"
#include <random>

namespace simbio {
	namespace systems {
		using namespace organism;

		void FlowerReproduction::registerFlowerReproductionSystem(World& world) {
		 	static std::mt19937 rng{ std::random_device{}() };
			static std::exponential_distribution<float> reproductionTimerDist{ 1.0f / Flower::REPRODUCTION_TIMER_SPREAD };
			static std::uniform_real_distribution<float> distanceDist{ -10.0f, 10.0f };
			static std::uniform_int_distribution<int> sizeChangeDist{ -1, 1 };

			world.flecsWorld.system().kind(flecs::OnUpdate).run([&](flecs::iter& it) {
				auto& bucket = reproductionQueue[currentReproductionTick];
        		std::vector<Entity> flowers;
        		flowers.swap(bucket);   
				for (auto flower : flowers) {
					flecs::entity e(world.flecsWorld, flower.flecsID);
					if (!e.is_alive()) continue;
					float childSize = std::clamp(flower.getSize() + sizeChangeDist(rng), Flower::MIN_SIZE, Flower::MAX_SIZE);
					float childRadius = childSize * 0.5f;
					float parentRadius = flower.getSize() * 0.5f;
					float sizeOffset = childRadius + parentRadius + 2.0f;
					float xOffset = distanceDist(rng);
					float yOffset = distanceDist(rng);
				
					float x = flower.location.x + std::copysign(sizeOffset, xOffset) + xOffset;
					float y = flower.location.y + std::copysign(sizeOffset, yOffset) + yOffset;
					x = x >= World::WIDTH ? 0.0f : x;
					y = y >= World::HEIGHT ? 0.0f : y;
					x = x < 0.0f ? World::WIDTH - 0.0001f : x;
					y = y < 0.0f ? World::HEIGHT - 0.0001f : y;

					int chunkX = (int)(x / World::CHUNK_SIZE);
					int chunkY = (int)(y / World::CHUNK_SIZE);
					int chunk = chunkY * world.chunkCols + chunkX;
					
					int reproductionTime = (int)std::ceil((Flower::REPRODUCTION_TIMER_SPREAD + reproductionTimerDist(rng)) / world.timeStep);
					reproductionQueue[(currentReproductionTick + reproductionTime) % Flower::MAX_REPRODUCTION_TICKS].push_back(flower);

					// This is a temporary optimization which limits flower population density.
					if (world.chunkGrid[chunk].size() >= 3) continue;

					world.spawnFlower(childSize, x, y);
				}

				currentReproductionTick = (currentReproductionTick + 1) % Flower::MAX_REPRODUCTION_TICKS;
			});

			world.flecsWorld.observer<Flower>().event(flecs::OnSet).each(
				[&](flecs::entity entity, Flower& flower) {
					int reproductionTime = (int)std::ceil((Flower::MIN_REPRODUCTION_TIMER + reproductionTimerDist(rng)) / world.timeStep);
					reproductionQueue[(currentReproductionTick + reproductionTime) % Flower::MAX_REPRODUCTION_TICKS].emplace_back(
						Entity{ .flecsID = entity.id(), .color = Flower::FLOWER_COLOR, .location = entity.get<Location>(), .organs = { .flower = flower } });
				}
			);
		}
	}
}
