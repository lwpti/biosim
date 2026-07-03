#pragma once

#include <flecs.h>
#include "data/Data.h"
#include <algorithm>
#include "organs/Organs.h"

namespace biosim {
	namespace organism {
        struct Entity {
			flecs::entity_t flecsID;
			Color color;
			Location location;
			Status status {0.0f, 0.0f };
			Velocity velocity{ 0.0f, 0.0f };
			Organs organs;

			float getSize() const {
				return (std::max)(Organs::sizeOf(organs.body), Organs::sizeOf(organs.flower));
			}
		};
    }
}