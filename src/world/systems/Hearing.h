#pragma once

#include "flecs.h"
#include <vector>
#include "organs/Body.h"
#include "organs/Legs.h"
#include "organs/Ears.h"
#include "data/Location.h"
#include "data/Velocity.h"
#include "percepts/Sound.h"
#include "Entity.h"

namespace simbio {
	namespace systems {
		class Hearing {
		public:
			static void registerHearingSystem(flecs::world& world, std::vector<std::vector<organism::Entity>>& chunkGrid,
				int chunkSize, int chunkCols, int chunkRows, float timeStep);
		};
	}
}
