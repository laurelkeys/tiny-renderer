#ifndef __MATH_HH__
#define __MATH_HH__

#include <limits>

namespace Math {

    ///////////////////////////////////////////////////////
    /// constants /////////////////////////////////////////
    ///////////////////////////////////////////////////////

    static const float MAX_INT      = std::numeric_limits<int>::max();
    static const float MIN_INT      = std::numeric_limits<int>::lowest(); // -MAX_INT - 1
    static const float MAX_FLOAT    = std::numeric_limits<float>::max();
    static const float MIN_FLOAT    = std::numeric_limits<float>::lowest(); // -MAX_FLOAT
    static const float EPS_FLOAT    = std::numeric_limits<float>::epsilon();

    static const float PI           = 3.14159265358979323846f;
    static const float INV_PI       = 0.31830988618379067154f;
    static const float INV_TWOPI    = 0.15915494309189533577f;
    static const float INV_FOURPI   = 0.07957747154594766788f;
    static const float SQRT_TWO     = 1.41421356237309504880f;
    static const float INV_SQRT_TWO = 0.70710678118654752440f;
    
    ///////////////////////////////////////////////////////
    /// common functions //////////////////////////////////
    ///////////////////////////////////////////////////////

    // Return -1 if val < 0, zero if val == 0, and +1 if x > 0
    template <typename T>
    inline int sgn(T val);

    // Constrain val to lie between min_val and max_val
    template <typename T>
    inline T clamp(T val, T min_val, T max_val);

    // Constrain val to lie between 0 and 1
    template <typename T>
    inline T saturate(T val);

    // Perform smooth (cubic Hermite) interpolation between 0 and 1 when min_val < val < max_val
    template <typename T>
    T smoothstep(T min_val, T max_val, T val);

    // Linearly interpolate between start and end using ratio to weight between them
    template <typename T>
    inline T lerp(T start, T end, T ratio);
}

#endif // __MATH_HH__