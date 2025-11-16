#pragma once

#include "Body.h"
#include "Legs.h"

namespace simbio {
	namespace organism {
		struct Organs {
			const Body* body;
			const Legs* legs;
		};

		struct Intents {
			const BodyIntent* body;
			const LegsIntent* legs;
		};
	}
}