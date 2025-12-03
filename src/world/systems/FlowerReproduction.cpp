#include "FlowerReproduction.h"

#include <algorithm>
#include <cmath>
#include "data/Location.h"
#include "Entity.h"

namespace simbio {
	namespace systems {
		using namespace organism;

		void FlowerReproduction::registerFlowerReproductionSystem(flecs::world& world, int worldWidth, int worldHeight,
			float timeStep, std::vector<std::vector<organism::Entity>>& chunkGrid, int chunkSize, int chunkCols, int chunkRows) {
			this->worldWidth = worldWidth;
			this->worldHeight = worldHeight;

		 	static std::mt19937 rng{ std::random_device{}() };
			static std::exponential_distribution<float> reproductionTimerDist{ 1.0f / Flower::MIN_REPRODUCTION_TIMER };
			static std::uniform_real_distribution<float> distanceDist{ -10.0f, 10.0f };
			static std::uniform_int_distribution<int> sizeChangeDist{ -1, 1 };

			world.system("FlowerReproductionSystem")
				.run([this, timeStep, &world, &chunkGrid, chunkSize, chunkCols, chunkRows](flecs::iter& it) {
				auto& bucket = reproductionQueue[currentReproductionTick];
        		std::vector<Entity> flowers;
        		flowers.swap(bucket);   
				for (auto flower : flowers) {
					flecs::entity e(world, flower.flecsID);
					if (!e.is_alive()) continue;
					float childSize = std::clamp(flower.getSize() + sizeChangeDist(rng), Flower::MIN_SIZE, Flower::MAX_SIZE);
					float childRadius = childSize * 0.5f;
					float parentRadius = flower.getSize() * 0.5f;
					float sizeOffset = childRadius + parentRadius + 2.0f;
					float xOffset = distanceDist(rng);
					float yOffset = distanceDist(rng);
				
					float x = flower.location.x + std::copysign(sizeOffset, xOffset) + xOffset;
					float y = flower.location.y + std::copysign(sizeOffset, yOffset) + yOffset;
					x = x >= this->worldWidth ? 0.0f : x;
					y = y >= this->worldHeight ? 0.0f : y;
					x = x < 0.0f ? this->worldWidth - 0.0001f : x;
					y = y < 0.0f ? this->worldHeight - 0.0001f : y;

					int chunkX = (int)(x / chunkSize);
					int chunkY = (int)(y / chunkSize);
					int chunk = chunkY * chunkCols + chunkX;
					// This is a temporary optimization which limits flower population density.
					if (chunkGrid[chunk].size() >= 3) {
						int reproductionTime = (int)std::ceil((Flower::REPRODUCTION_TIMER_SPREAD + reproductionTimerDist(rng)) / timeStep);
						reproductionQueue[(currentReproductionTick + reproductionTime) % Flower::MAX_REPRODUCTION_TICKS].push_back(flower);
						continue;
					}

					bool overlaps = false;
					for (int dy = -1; dy <= 1 && !overlaps; ++dy) {
						int adjX = chunkY + dy;
						adjX = adjX < 0 ? chunkRows - 1 : adjX;
						adjX = adjX >= chunkRows ? 0 : adjX;

						for (int dx = -1; dx <= 1 && !overlaps; ++dx) {
							int adjY = chunkX + dx;
							adjY = adjY < 0 ? chunkCols - 1 : adjY;
							adjY = adjY >= chunkCols ? 0 : adjY;

							auto& bucket = chunkGrid[adjX * chunkCols + adjY];
							for (int i = 0; i < bucket.size(); ++i) {
								float distanceX = bucket[i].location.x - x;
								float distanceY = bucket[i].location.y - y;
								float minDistance = childRadius + 2.0f + bucket[i].getSize() * 0.5f;

								if (distanceX * distanceX + distanceY * distanceY <= minDistance * minDistance) {
									overlaps = true;
									break;
								}
							}
						}
					}

					if (!overlaps) {
						Location location = { x, y, 0.0f, chunk };

						flecs::entity child = world.entity()
							.set<Flower>(Flower{ (float)childSize })
							.set<simbio::organism::Color>(Flower::FLOWER_COLOR)
							.set<Location>(location);

						int reproductionTime = (int)std::ceil((Flower::REPRODUCTION_TIMER_SPREAD + reproductionTimerDist(rng)) / timeStep);
						Entity e = Entity{ .flecsID = child.id(), .location = location, .organs = Organs{ .flower = childSize } };
						reproductionQueue[(currentReproductionTick + reproductionTime) % Flower::MAX_REPRODUCTION_TICKS].push_back(e);

						chunkGrid[chunk].push_back(e);
					}
						
					int reproductionTime = (int)std::ceil((Flower::REPRODUCTION_TIMER_SPREAD + reproductionTimerDist(rng)) / timeStep);
					reproductionQueue[(currentReproductionTick + reproductionTime) % Flower::MAX_REPRODUCTION_TICKS].push_back(flower);
				}

				currentReproductionTick = (currentReproductionTick + 1) % Flower::MAX_REPRODUCTION_TICKS;
			});		
		}
	}
}
