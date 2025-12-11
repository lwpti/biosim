#pragma once

#include "flecs.h"
#include <vector>
#include "Organism.h"
#include <array>
#include "Entity.h"
#include "World.h"

namespace simbio {
	namespace systems {
		class Death {
		public:
			/// <summary>
			/// Registers the Death flecs system. 
			/// </summary>
			static void registerDeathSystem(World& world);

			/// <summary>
			/// Registers the FlowerDeath flecs system. 
			/// </summary>
			static void registerFlowerDeathSystem(World& world);

			static constexpr float MIN_DEATH_TIMER = 900.0f;
			static constexpr float DEATH_TIMER_SPREAD = 900.0f;
			static constexpr int MAX_DEATH_TICKS = 4.0f * (MIN_DEATH_TIMER + DEATH_TIMER_SPREAD) / 0.1f;
			inline static std::array<std::vector<organism::Entity>, MAX_DEATH_TICKS> deathQueue;

		private:
			inline static int deathTick = 0;
		};
	}
}
