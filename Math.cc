#include <algorithm>

#include "Math.hh"

namespace Math {

    template <typename T>
    inline int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    template <typename T>
    inline T clamp(T val, T min_val, T max_val) {
        return std::min(std::max(val, min_val), max_val);
    }

    template <typename T>
    inline T saturate(T val) {
        return clamp(val, T(0), T(1));
    }

    template <typename T>
    T smoothstep(T min_val, T max_val, T val) {
        // scale, bias and saturate val to 0..1 range
        T t = saturate((val - min_val) / (max_val - min_val));
        return t * t * (T(3) - T(2) * t); // evaluate polynomial
    }

    template <typename T>
    inline T lerp(T start, T end, T ratio) {
        return (T(1) - ratio) * start + ratio * end; // start + ratio * (end - start)
    }
}