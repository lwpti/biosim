#pragma once

#include <raylib.h>
#include <random>

namespace simbio {
	namespace plants {
		struct Flower {
			int size;
			Color color;
			int reproductionTimer;
			int deathTimer;

			constexpr static int MIN_SIZE = 1, MAX_SIZE = 5;
			constexpr static float MIN_REPRODUCTION_TIMER = 90.0f, REPRODUCTION_TIMER_SPREAD = 90.0f;
			constexpr static float MIN_DEATH_TIMER = 900.0f, DEATH_TIMER_SPREAD = 900.0f;

			static void registerFlowerObserver(flecs::world& world, float timeStep) {
				static std::mt19937 rng{ std::random_device{}() };
            	static std::exponential_distribution<float> flowerDeathTimerDist{ 1.0f / Flower::DEATH_TIMER_SPREAD };
				static std::exponential_distribution<float> reproductionTimerDist(1.0f / Flower::MIN_REPRODUCTION_TIMER);
				static std::uniform_int_distribution<int> sizeDist(Flower::MIN_SIZE, Flower::MAX_SIZE);
				static std::uniform_real_distribution<float> unitDist(0.0f, 1.0f);

				world.observer<Flower>("FlowerObserver")
					.event(flecs::OnAdd)
					.each([timeStep](Flower& flower) {
							flower.reproductionTimer = (int)std::ceil((Flower::REPRODUCTION_TIMER_SPREAD + reproductionTimerDist(rng)) / timeStep);
							flower.deathTimer = (int)std::ceil((Flower::MIN_DEATH_TIMER + flowerDeathTimerDist(rng)) / timeStep);
						});
			}
		};
	}
}
