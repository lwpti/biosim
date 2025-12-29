#pragma once

namespace biosim {
    namespace organism {
        /// <summary>
        /// flecs component; Arms determine how strongly an organism can hold another.
        /// </summary>
        struct Arms {
			float size;

			static constexpr float MIN_SIZE = 0.0f, MAX_SIZE = 5.0f;
        };

        /// <summary>
        /// Grab intent:
        /// grab = true -> try to grab something
        /// grab = false -> release if currently grabbing
        /// </summary>
        struct ArmsIntent {
            bool grab = false;
        };

        /// <summary>
        /// Which entity this organism is grabbing.
        /// </summary>
        struct Grabbing {
        };
    }
}
