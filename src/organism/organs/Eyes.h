#pragma once

namespace biosim {
	namespace organism {
		/// <summary>
		/// flecs component; allows an organism to perceive sight.
		/// </summary>
		struct Eyes {
			float size;

			constexpr static float MIN_SIZE = 0.0f, MAX_SIZE = 5.0f;			
		};
	}
}