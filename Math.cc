#include "Math.hh"

namespace Math {
    
    float deg2rad(float angle_deg) {
        return angle_deg * (PI / 180.0f);
    }

    float rad2deg(float angle_rad) {
        return angle_rad * (180.0f / PI);
    }
}