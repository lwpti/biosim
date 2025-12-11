#pragma once

#include "World.h"

namespace simbio {
	namespace systems {
		class Movement {
		public:
			/// <summary>
			/// Registers the MoveIntent flecs system. 
			/// </summary>
			static void registerMoveIntentSystem(World& world);

			/// <summary>
			/// Registers the Movement flecs system.
			/// </summary>
			static void registerMovementSystem(World& world);
		};
	}
}
