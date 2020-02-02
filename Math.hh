#ifndef __MATH_HH__
#define __MATH_HH__

#include <algorithm>
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

    template <typename T>
    T min(const T &a, const T &b) {
        return a < b ? a : b;
    }

    template <typename T, typename... Ts>
    T min(const T &a, const T &b, const Ts &... ts) {
        return min(min(a, b), ts...);
    }

    template <typename T>
    T max(const T &a, const T &b) {
        return a > b ? a : b;
    }

    template <typename T, typename... Ts>
    T max(const T &a, const T &b, const Ts &... ts) {
        return max(max(a, b), ts...);
    }

    // Return -1 if val < 0, zero if val == 0, and +1 if x > 0
    template <typename T>
    inline int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    // Constrain val to lie between min_val and max_val
    template <typename T>
    inline T clamp(T val, T min_val, T max_val) {
        return std::min(std::max(val, min_val), max_val);
    }

    // Constrain val to lie between 0 and 1
    template <typename T>
    inline T saturate(T val) {
        return clamp(val, T(0), T(1));
    }

    // Perform smooth (cubic Hermite) interpolation between 0 and 1 when min_val < val < max_val
    template <typename T>
    T smoothstep(T min_val, T max_val, T val) {
        // scale, bias and saturate val to 0..1 range
        T t = saturate((val - min_val) / (max_val - min_val));
        return t * t * (T(3) - T(2) * t); // evaluate polynomial
    }

    // Linearly interpolate between start and end using ratio to weight between them
    template <typename T>
    inline T lerp(T start, T end, T ratio) {
        return (T(1) - ratio) * start + ratio * end; // start + ratio * (end - start)
    }
}

#endif // __MATH_HH__