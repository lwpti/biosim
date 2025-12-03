#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; the Mouth allows organisms to damage and eat other organisms and
		/// plants, and to produce sound.
		/// </summary>
		struct Mouth {
			float size;

			constexpr static float MIN_SIZE = 0, MAX_SIZE = 5;
		};

		/// <summary>
		/// flecs component; describes a desire to bite.
		/// </summary>
		struct BiteIntent {
		};

		/// <summary>
		/// flecs component; describes a desire to make noise.
		/// </summary>
		struct CryIntent {
		};
	}
}