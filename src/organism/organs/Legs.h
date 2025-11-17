#pragma once

namespace simbio {
	namespace organism {
		struct Legs {
			int size;
		};

		struct LegsIntent {
			float direction;
			float magnitude;
			float turn;
		};
	}
}