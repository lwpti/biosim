#pragma once

namespace biosim {
	namespace organism {
		/// <summary>
		/// flecs component; describes a 24-bit color.
		/// </summary>
		struct Color {
			unsigned char r, g, b;
		};
	}
}