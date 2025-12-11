#include "Location.h"

namespace simbio {
    namespace organism {
        Location Location::lerp(const Location& from, const Location& to, float t) {
            return Location{
                from.x + (to.x - from.x) * t,
                from.y + (to.y - from.y) * t,
                from.yaw + (to.yaw - from.yaw) * t,
                from.chunk // Note: chunk is not interpolated
            };
        }
    }
}