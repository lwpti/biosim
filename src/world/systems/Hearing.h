#pragma once

#include "World.h"

namespace biosim {
	namespace systems {
		class Hearing {
		public:
			static void registerHearingSystem(World& world);
		};
	}
}
