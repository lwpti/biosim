#include "FlowerReproduction.h"

#include <algorithm>
#include <cmath>

#include "data/Location.h"

namespace simbio {
	namespace systems {
		using namespace plants;
		using namespace organism;

		void FlowerReproduction::registerFlowerReproductionSystem(flecs::world& world, int worldWidth, int worldHeight,
			float timeStep, std::vector<std::vector<flecs::entity_t>>& chunkGrid, int chunkSize, int chunkCols, int chunkRows) {
			this->worldWidth = worldWidth;
			this->worldHeight = worldHeight;

		 	static std::mt19937 rng{ std::random_device{}() };
			static std::exponential_distribution<float> reproductionTimerDist{ 1.0f / plants::Flower::MIN_REPRODUCTION_TIMER };
			static std::uniform_real_distribution<float> distanceDist{ -15.0f, 15.0f };
			static std::uniform_int_distribution<int> sizeChangeDist{ -1, 1 };

			world.system<Flower, Location>("FlowerReproductionSystem")
				.each([this, timeStep, &world, &chunkGrid, chunkSize, chunkCols, chunkRows]
					(flecs::entity e, Flower& flower, const Location& location) {
					if (flower.reproductionTimer > 0) {
						--flower.reproductionTimer;
					} else if (flower.reproductionTimer == 0) {
						int childSize = std::clamp(flower.size + sizeChangeDist(rng), Flower::MIN_SIZE, Flower::MAX_SIZE);

						float x = std::clamp(location.x + distanceDist(rng), 0.0f, (float)this->worldWidth - 0.0001f);
						float y = std::clamp(location.y + distanceDist(rng), 0.0f, (float)this->worldHeight - 0.0001f);

						int chunkX = (int)(x / chunkSize);
						int chunkY = (int)(y / chunkSize);
						int chunk = chunkY * chunkCols + chunkX;

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
									flecs::entity adjEntity(world, bucket[i]);

									const Flower* adjFlower = adjEntity.try_get<Flower>();
									if (!adjFlower) continue;

									const Location* adjLocation = adjEntity.try_get<Location>();
									if (!adjLocation) continue;

									float distanceX = adjLocation->x - x;
									float distanceY = adjLocation->y - y;
									float minDistance = childSize * 0.5f + 2.0f + adjFlower->size * 0.5f;

									if (distanceX * distanceX + distanceY * distanceY <= minDistance * minDistance) {
										overlaps = true;
										break;
									}
								}
							}
						}

						if (!overlaps) {
							flecs::entity child = world.entity()
								.set<Flower>(Flower{ childSize, flower.color })
								.set<Location>({ x, y, 0.0f, chunk });

							chunkGrid[chunk].push_back(child.id());
						}

						flower.reproductionTimer = (int)std::ceil((Flower::MIN_REPRODUCTION_TIMER + reproductionTimerDist(rng)) / timeStep);
					}
				});
		}

	}
}
