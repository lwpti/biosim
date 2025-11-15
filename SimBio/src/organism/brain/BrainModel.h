#pragma once

#include <vector>
#include <variant>
#include "Organism.h"

namespace simbio {
	namespace organism {
		class BrainModel {
		public:
			virtual ~BrainModel() = default;
			virtual std::vector<Intent> think(std::vector<Organ>) = 0;

		protected:
			BrainModel() {}
		};
	}
}