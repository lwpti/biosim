#pragma once

#include "flecs.h"
#include <vector>

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
			/// <param name="world">flecs world which the system is registed in</param>
			void registerMoveIntentSystem(flecs::world& world);

			/// <summary>
			/// Registers the Movement flecs system.
			/// The system parameters are const Move&, Location&, and Velocity&.
			/// This system applies the movement described in the Move component by
			/// modifying Location and Velocity.
			/// </summary>
			/// <param name="world">flecs world which the system is registed in</param>
			/// <param name="worldWidth">Width of the world</param>
			/// <param name="worldHeight">Height of the world</param>
			/// <param name="timeStep">Time between world state progressions</param>
			/// <param name="chunkGrid">Reference to the chunk grid to update organism chunk positions</param>
			/// <param name="chunkSize">Size of each chunk in the chunk grid</param>
			/// <param name="chunkCols">Number of columns in the chunk grid</param>
			void registerMovementSystem(flecs::world& world, int worldWidth, int worldHeight, 
				float timeStep, std::vector<std::vector<flecs::entity_t>>& chunkGrid, int chunkSize, int chunkCols);

		private:
			/// <summary>
			/// Describes a 2D movement - acceleration magnitudue, x and y components, and yaw.
			/// </summary>
			struct Move {
				struct {
					float magnitude, x, y;
				} a;
				float yaw;
			};
		};
	}
}
