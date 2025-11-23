#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; Arms determine how strongly an organism can hold another.
		/// </summary>
		struct Arms {
			float size;
		};

		/// <summary>
		/// flecs component; describes a desire to attempt to grab.
		/// </summary>
		struct ArmsIntent {
		};
	}
}