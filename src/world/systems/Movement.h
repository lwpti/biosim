#pragma once

#include "flecs.h"

namespace simbio {
	namespace systems {
		class Movement {
		public:
			/// <summary>
			/// Registers the MovIntent flecs system. 
			/// The system parameters are const Legs& and const LegsIntent&.
			/// This system verifies that the LegsIntent is possible given the size of
			/// the Legs (and restricts it if not) then adds a Move component.
			/// </summary>
			/// <param name="world">flecs world which the system is registed in</param>
			void registerMoveIntentSystem(flecs::world& world);

			/// <summary>
			/// Registers the Movement flecs system.
			/// The system parameters are const Move&, Location&, and Velocity&.
			/// This system applies the movement described in the Move component by
			/// modifying Location and Velocity.
			/// </summary>
			/// <param name="world">flecs world which the system is registed in</param>
			void registerMovementSystem(flecs::world& world, int worldWidth, int worldHeight);

		private:
			/// <summary>
			/// Describes a 2D movement - acceleration direction and magnitude, and
			/// angular acceleration.
			/// </summary>
			struct Move {
				float aDirection;
				float aMagnitude;
				float aAngular;
			};

			int worldWidth;
			int worldHeight;
		};
	}
}