#pragma once

#include "flecs.h"
#include <vector>
#include "Organism.h"
#include <array>

namespace simbio {
	namespace systems {
		class Death {
		public:
			/// <summary>
			/// Registers the Death flecs system. 
			/// The system parameter is const Status& and const Location&.
			/// This system deletes organisms with 0 health or energy and removes them from the chunk grid.
			/// </summary>
			/// <param name="world">flecs world which the system is registed in</param>
			/// <param name="chunkGrid">Reference to the chunk grid to remove dead organisms from</param>
			void registerDeathSystem(flecs::world& world, std::vector<std::vector<organism::SimpleEntity>>& chunkGrid);

			/// <summary>
			/// Registers the FlowerDeath flecs system. 
			/// The system parameter is Flower& and const Location&.
			/// This system decrements the flower's death timer and deletes the flower when it reaches 0 and
			/// removes it from the chunk grid.
			/// </summary>
			/// <param name="world">flecs world which the system is registed in</param>
			/// <param name="chunkGrid">Reference to the chunk grid to remove dead flowers from</param>
			void registerFlowerDeathSystem(flecs::world& world, std::vector<std::vector<organism::SimpleEntity>>& chunkGrid);

		private:
			constexpr static float MIN_DEATH_TIMER = 900.0f, DEATH_TIMER_SPREAD = 900.0f;
			constexpr static int MAX_DEATH_TICKS = 4.0f * (MIN_DEATH_TIMER + DEATH_TIMER_SPREAD) / 0.1f;
			std::array<std::vector<organism::SimpleEntity>, MAX_DEATH_TICKS> deathQueue;
			int deathTick = 0;
		};
	}
}
