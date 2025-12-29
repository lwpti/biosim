#pragma once

#include "Sight.h"
#include "Sound.h"
#include "Touch.h"
#include <optional>

namespace biosim {
    namespace organism {
        /// <summary>
        /// Convenient structure grouping percept structs.
        /// </summary>
        struct Percepts {
            std::optional<Sight> sight;
            std::optional<Sound> sound;
        };
    }
}
