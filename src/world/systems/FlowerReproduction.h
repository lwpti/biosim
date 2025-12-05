#pragma once

#include <vector>
#include <array>
#include "organs/Flower.h"
#include "Entity.h"
#include "World.h"

namespace simbio {
	namespace systems {
		class FlowerReproduction {
		public:
			/// <summary>
			/// Registers a system that allows Flowers to reproduce.
			/// </summary>
			/// <param name="world">flecs world which the system is registered in</param>
			/// <param name="worldWidth">Width of the world</param>
			/// <param name="worldHeight">Height of the world</param>
			/// <param name="timeStep">Time between world state progressions</param>
			/// <param name="chunkGrid">Reference to the chunk grid to place new Flowers into</param>
			/// <param name="chunkSize">Size of each chunk in the chunk grid</param>
			/// <param name="chunkCols">Number of columns in the chunk grid</param>
			/// <param name="chunkRows">Number of rows in the chunk grid</param>
			static void registerFlowerReproductionSystem(World& world);

			static inline std::array<std::vector<organism::Entity>, organism::Flower::MAX_REPRODUCTION_TICKS> reproductionQueue{};
			static inline int currentReproductionTick = 0;
		};
	}
}
