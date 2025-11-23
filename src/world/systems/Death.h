#pragma once

#include "flecs.h"

namespace simbio {
	namespace systems {
		class Death {
		public:

			/// <summary>
			/// Registers the Death flecs system. 
			/// The system parameter is const Status&.
			/// This system deletes organisms with 0 health or energy.
			/// </summary>
			/// <param name="world">flecs world which the system is registed in</param>
			void registerDeathSystem(flecs::world& world);
		};
	}
}