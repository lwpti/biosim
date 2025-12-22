#include "Location.h"
#include "raylib.h"
#include <cmath>

namespace simbio {
    namespace organism {
        Location Location::lerp(const Location& from, const Location& to, float t) {
            if (from.yaw < 0 && to.yaw > 0 && std::fabs(from.yaw - to.yaw) > PI) {
                return lerp(from, Location{ to.x, to.y, to.yaw - 2.0f * PI, to.chunk }, t);
            } else if (from.yaw > 0 && to.yaw < 0 && std::fabs(from.yaw - to.yaw) > PI) {
                return lerp(Location{ from.x, from.y, from.yaw - 2.0f * PI, from.chunk }, to, t);
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