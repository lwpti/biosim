#pragma once

#include "flecs.h"

namespace simbio {
	namespace systems {
		class Display {
		public:
			Display(flecs::world& world, int width, int height);

		private:
			const int width;
			const int height;
		};
	}
}