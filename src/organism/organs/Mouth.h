#pragma once

namespace simbio {
	class World;

	namespace organism {
		/// <summary>
		/// flecs component; the Mouth allows organisms to damage and eat other organisms and
		/// plants, and to produce sound.
		/// </summary>
		struct Mouth {
			float size;
			float power;

			constexpr static float MIN_SIZE = 0.0f, MAX_SIZE = 5.0f;
			constexpr static float MIN_POWER = 1.0f, MAX_POWER = 5.0f;
			constexpr static float MAX_OVERSIZE = 1.5f;

			static void registerMouthObserver(World& world);
		};

		/// <summary>
		/// flecs component; describes a desire to bite.
		/// </summary>
		struct BiteIntent {
			float strength;
		};

		/// <summary>
		/// flecs component; describes a desire to make noise.
		/// </summary>
		struct CryIntent {
		};
	}
}