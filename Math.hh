#ifndef __MATH_HH__
#define __MATH_HH__

#include <algorithm>

#include "Types.hh"

namespace Math {

    // Return -1 if val < 0, zero if val == 0, and +1 if x > 0.
    template <typename T>
    int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    // Constrain val to lie between min_val and max_val
    template <typename T>
    T clamp(T val, T min_val, T max_val) {
        return std::min(std::max(val, min_val), max_val);
    }

    // Constrain val to lie between 0 and 1
    template <typename T>
    T saturate(T val) {
        return clamp(val, T(0), T(1));
    }

    // Perform smooth (cubic Hermite) interpolation between 0 and 1 when min_val < val < max_val
    template <typename T>
    T smoothstep(T min_val, T max_val, T val) {
        // scale, bias and saturate x to 0..1 range
        T t = saturate((val - min_val) / (max_val - min_val));
        return t * t * (T(3) - T(2) * t); // evaluate polynomial
    }

    // Linearly interpolate between start and end using ratio to weight between them
    template <typename T>
    T lerp(T start, T end, T ratio) {
        return (T(1) - ratio) * start + ratio * end; // start + ratio * (end - start)
    }
}

#endif // __MATH_HH__