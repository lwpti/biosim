#pragma once

#include <optional>
#include "Body.h"
#include "Legs.h"

namespace simbio {
	namespace organism {
		struct Organs {
			std::optional<Body> body;
			std::optional<Legs> legs;
		};

		struct Intents {
			std::optional<BodyIntent> body;
			std::optional<LegsIntent> legs;
		};
	}
}