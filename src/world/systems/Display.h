#pragma once

#include "flecs.h"

namespace simbio {
	namespace systems {
		class Display {
		public:
			Display(int width, int height);
			void registerDrawSystems(flecs::world& world) const;

		private:
			const int width;
			const int height;
		};
	}
}