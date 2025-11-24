#include "FlowerReproduction.h"

#include <algorithm>
#include <cmath>
#include <random>

#include "plants/Flower.h"
#include "data/Location.h"

namespace simbio {
	namespace systems {
		using namespace plants;
		using namespace organism;

		void FlowerReproduction::registerFlowerReproductionSystem(flecs::world& world, int worldWidth, int worldHeight, float timeStep) {
			this->worldWidth = worldWidth;
			this->worldHeight = worldHeight;

			std::random_device rd;
			std::mt19937 rng(rd());
			std::exponential_distribution<float> reproductionTimerDist(1.0f / 30.0f);
			std::uniform_real_distribution<float> offset(-15.0f, 15.0f);
			std::uniform_int_distribution<int> sizeDelta(-1, 1);

			world.system<Flower, Location>("FlowerReproductionSystem")
				.each([this, &offset, &sizeDelta, &rng, world, &reproductionTimerDist, &timeStep](flecs::entity e, Flower& flower, const Location& location) {
					if (flower.reproductionTimer > 0) {
						--flower.reproductionTimer;
					}

					if (flower.reproductionTimer <= 0) {
						int childSize = std::clamp(flower.size + sizeDelta(rng), Flower::MIN_SIZE, Flower::MAX_SIZE);

						float newX = std::clamp(location.x + offset(rng), 0.0f, static_cast<float>(this->worldWidth));
						float newY = std::clamp(location.y + offset(rng), 0.0f, static_cast<float>(this->worldHeight));

						//world.entity()
						//	.set<Flower>(Flower{
						//		childSize,
						//		flower.color,
						//		(int)std::ceil(15.0f + reproductionTimerDist(rng) / timeStep)
						//	})
						//	.set<Location>({ newX, newY, 0.0f });
						//flower.reproductionTimer = (int)std::ceil(15.0f + reproductionTimerDist(rng) / timeStep);
					}
				});
		}
	}
}
