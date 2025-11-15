#pragma once

namespace simbio {
	namespace organism {
		struct Legs {
			int size;
		};

		struct LegsIntent {
			int direction;
			int magnitude;
			int turn;
		};
	}
}