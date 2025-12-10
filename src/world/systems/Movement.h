#pragma once

#include "World.h"

namespace simbio {
	namespace systems {
		class Movement {
		public:
			/// <summary>
			/// Registers the MoveIntent flecs system. 
			/// The system parameters are const Legs& and const LegsIntent&.
			/// This system verifies that the LegsIntent is possible given the size of
			/// the Legs (and restricts it if not) then adds a Move component.
			/// </summary>
			static void registerMoveIntentSystem(World& world);

			/// <summary>
			/// Registers the Movement flecs system.
			/// The system parameters are const Move&, Location&, and Velocity&.
			/// This system applies the movement described in the Move component by
			/// modifying Location and Velocity.
			/// </summary>
			static void registerMovementSystem(World& world);
		};
	}
}
