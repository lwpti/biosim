#pragma once

#include <vector>
#include <array>
#include "organs/Flower.h"
#include "Entity.h"
#include "World.h"

namespace biosim {
	namespace systems {
		class FlowerReproduction {
		public:
			/// <summary>
			/// Registers a system that allows Flowers to reproduce.
			/// </summary>
			static void registerFlowerReproductionSystem(World& world);

			static inline std::array<std::vector<organism::Entity>, organism::Flower::MAX_REPRODUCTION_TICKS> reproductionQueue{};
			static inline int currentReproductionTick = 0;
		};
	}
}
