#include "Math.hh"

#include <math.h>

namespace Math {

    float deg2rad(float angle_deg) {
        return angle_deg * (PI / 180.0f);
    }

    float rad2deg(float angle_rad) {
        return angle_rad * (180.0f / PI);
    }

    float srgb2linear(float color_srgb) {
        return pow(color_srgb, 2.2f);
    }

    float linear2srgb(float color_linear) {
        return pow(color_linear, 1.0f / 2.2f);
    }
}