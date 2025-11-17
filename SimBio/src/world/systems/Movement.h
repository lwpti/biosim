#pragma once

#include "flecs.h"

namespace simbio {
	namespace systems {
		class Movement {
		public:
			void registerMoveIntentSystem(flecs::world& world);
			void registerMovementSystem(flecs::world& world);
		};
	}
}