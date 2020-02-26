#ifndef __GEOMETRY_HH__
#define __GEOMETRY_HH__

#include "Math.hh"
#include "Types.hh"

namespace Geometry {

    struct PointProps {
        bool is_inside_triangle; // true iff all barycentric_coords are positive
        Types::Vec3f barycentric_coords; // (1-u-v, u, v)
    };

    // Returns the barycentric coordinates of point p with respect to the triangle abc,
    // which express any position on the plane (p) as a linear combination of the three vertices positions (a, b, c)
    // obs.: the sum of the barycentric coordinates is 1, and p only lies inside abc iff all coordinates are positive
    PointProps barycentric_coords(
        const Types::Vec2i &p,
        const Types::Vec2i &a, const Types::Vec2i &b, const Types::Vec2i &c
    );

    // Uses the barycentric coordinates as interpolation coefficients
    // to perform a weighted mix of the values of the triangle vertices
    // ref.: https://fgiesen.wordpress.com/2013/02/06/the-barycentric-conspirac/
    template <typename T>
    float barycentric_interp(
        const Types::Vec3f &coords,
        const Types::Vec3<T> &abc
    ) {
        return coords.x * static_cast<float>(abc.x) +
               coords.y * static_cast<float>(abc.y) +
               coords.z * static_cast<float>(abc.z); // (1-u-v) * a + u * b + v * c
    }

    template <typename T>
    Types::Vec2f barycentric_interp(
        const Types::Vec3f &coords,
        const Types::Vec2<T> &a, const Types::Vec2<T> &b, const Types::Vec2<T> &c
    ) {
        return coords.x * Types::Vec2<float>(a) +
               coords.y * Types::Vec2<float>(b) +
               coords.z * Types::Vec2<float>(c); // (1-u-v) * a + u * b + v * c
    }

    template <typename T>
    Types::Vec3f barycentric_interp(
        const Types::Vec3f &coords,
        const Types::Vec3<T> &a, const Types::Vec3<T> &b, const Types::Vec3<T> &c
    ) {
        return coords.x * Types::Vec3<float>(a) +
               coords.y * Types::Vec3<float>(b) +
               coords.z * Types::Vec3<float>(c); // (1-u-v) * a + u * b + v * c
    };
}

#endif // __GEOMETRY_HH__