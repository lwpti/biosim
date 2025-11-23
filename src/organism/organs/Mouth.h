#pragma once

namespace simbio {
	namespace organism {
		/// <summary>
		/// flecs component; the Mouth allows organisms to damage and eat other organisms and
		/// plants, and to produce sound.
		/// </summary>
		struct Mouth {
			int size;
		};

		/// <summary>
		/// flecs component; describes a desire to bite or make noise.
		/// </summary>
		struct MouthIntent {
		};
	}
}