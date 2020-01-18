#ifndef __ANGLE_HH__
#define __ANGLE_HH__

const float PI = 3.1415926536f;
const float TWO_PI = 2.0f * PI;
const float INV_PI = 1.0f / PI;

struct Angle {
    static const float degrees(float angle_in_radians) {
        return angle_in_radians * (180.0f / PI);
    }

    static const float radians(float angle_in_degrees) {
        return angle_in_degrees * (PI / 180.0f);
    }
};

#endif // __ANGLE_HH__