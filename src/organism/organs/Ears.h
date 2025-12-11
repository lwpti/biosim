#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; allows an organism to perceive sound.
		/// </summary>
		struct Ears {
			float size;

			constexpr static float MIN_SIZE = 0.0f, MAX_SIZE = 5.0f;			
		};
	}
}