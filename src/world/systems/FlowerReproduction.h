#pragma once

#include "flecs.h"

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
			void registerFlowerReproductionSystem(flecs::world& world, int worldWidth, int worldHeight, float timeStep);

		private:
			int worldWidth;
			int worldHeight;
		};
	}
}
