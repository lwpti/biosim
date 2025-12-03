#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; Arms determine how strongly an organism can hold another.
		/// </summary>
		struct Arms {
			float size;

			constexpr static float MIN_SIZE = 0, MAX_SIZE = 5;
		};

		/// <summary>
		/// flecs component; describes a desire to attempt to grab.
		/// </summary>
		struct ArmsIntent {
		};
	}
}