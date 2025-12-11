#pragma once

#include "Arms.h"
#include "Body.h"
#include "Ears.h"
#include "Eyes.h"
#include "Legs.h"
#include "Mouth.h"
#include "Flower.h"

namespace simbio {
    namespace organism {
        /// <summary>
        /// Convenient structure grouping all organ components.
        /// Organ.size = 0 means the organ is not present.
        /// </summary>
        struct Organs {
            Body body{ 0.0f };
            Legs legs{ 0.0f };
            Mouth mouth{ 0.0f };
            Arms arms{ 0.0f };
            Ears ears{ 0.0f };
            Eyes eyes{ 0.0f };
            Flower flower{ 0.0f };
        };
    }
}