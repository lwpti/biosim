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
