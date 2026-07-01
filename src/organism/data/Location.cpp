#include "Location.h"
#include "Constants.h"
#include <cmath>

namespace biosim {
    namespace organism {
        Location Location::lerp(const Location& from, const Location& to, float t) {
            if (from.yaw < 0 && to.yaw > 0 && std::fabs(from.yaw - to.yaw) > PI_F) {
                return lerp(from, Location{ to.x, to.y, to.yaw - TWO_PI_F, to.chunk }, t);
            } else if (from.yaw > 0 && to.yaw < 0 && std::fabs(from.yaw - to.yaw) > PI_F) {
                return lerp(Location{ from.x, from.y, from.yaw - TWO_PI_F, from.chunk }, to, t);
            }

            return Location{
                from.x + (to.x - from.x) * t,
                from.y + (to.y - from.y) * t,
                from.yaw + (to.yaw - from.yaw) * t,
                from.chunk // Note: chunk is not interpolated
            };
        }
    }
}