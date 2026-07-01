#pragma once

#include "World.h"
#include "Constants.h"

namespace biosim {
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

			static constexpr float MAX_S = 250.0f;
			inline static const float MAX_A = 250.0f;
			inline static const float MAX_DA = -1.5f * MAX_A;
			inline static const float MAX_TA = 1.0f * MAX_A;
			inline static const float MAX_AXIS_A = MAX_A / std::sqrt(2.0f);
			inline static const float MAX_AXIS_DA = -1.5f * MAX_AXIS_A;
			inline static const float MAX_AXIS_TA = 1.0f * MAX_AXIS_A;
			static constexpr float MAX_YAW = 3.0f * PI_F;
			// Moving consumes ~43% of energy if used continuously, and total usage ~= 2/s
            static constexpr float MOVE_ENERGY_COST = 0.86f;
			// Yaw consumes ~1% of energy if used continuously, and total usage ~= 2/s
            static constexpr float YAW_ENERGY_COST = 0.02f;
		};
	}
}
