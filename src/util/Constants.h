#pragma once

#include <cmath>
#include <numbers>

namespace biosim {
    inline constexpr float PI_F = std::numbers::pi_v<float>;
    inline constexpr float TWO_PI_F = 2.0f * PI_F;

    inline float normalizeRadians(float radians) {
        radians = std::fmod(radians + PI_F, TWO_PI_F);
        if (radians <= 0.0f) radians += TWO_PI_F;
        return radians - PI_F;
    }
}