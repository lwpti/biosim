#pragma once

#include <variant>

#include "Location.h"

#include "Body.h"
#include "Legs.h"

namespace simbio {
	namespace organism {
		using Organ = std::variant<Body, Legs>;
		using Intent = std::variant<BodyIntent, LegsIntent>;
	}
}