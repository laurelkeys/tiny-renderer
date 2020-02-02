#ifndef __GEOMETRY_HH__
#define __GEOMETRY_HH__

#include "Math.hh"
#include "Types.hh"

namespace Geometry {

    template <typename T>
    struct Triangle2D {
        Types::Vec2<T> a, b, c; // vertices positions

        Triangle2D() = delete;
        explicit Triangle2D(Types::Vec2<T> a, Types::Vec2<T> b, Types::Vec2<T> c)
            : a(a)
            , b(b)
            , c(c) { }

        Types::Vec3f barycentric_coords(const Types::Vec2<T> &p) {
            Types::Vec2<T> AB = b - a, AC = c - a, PA = a - p;
            Types::Vec3<T> coords = cross(
                Types::Vec3<T>(AC.x, AB.x, PA.x),
                Types::Vec3<T>(AC.y, AB.y, PA.y)
            );

            float area2 = static_cast<float>(coords.z); // 2 * area(a, b, c)

            if (std::abs(area2) <= Math::EPS_FLOAT) {
                // the triangle is degenerate (area == 0), so we
                // return a negative coordinate for it to be skipped, since
                // p is inside the triangle iff 0 <= u, v <= 1 and u + v <= 1
                return Types::Vec3f(-1, 1, 1);
            }

            return Types::Vec3f(
                1 - (coords.y + coords.x) / area2, // 1 - u - v
                coords.y / area2, // u = (PA.x * AC.y - AC.x * PA.y) / (AC.x * AB.y - AB.x * AC.y)
                coords.x / area2  // v = (AB.x * PA.y - AB.y * PA.x) / (AC.x * AB.y - AB.x * AC.y)
            );
        }
    };

    template <typename T>
    struct Triangle3D {
        Types::Vec3<T> a, b, c; // vertices positions

        Triangle3D() = delete;
        explicit Triangle3D(Types::Vec3<T> a, Types::Vec3<T> b, Types::Vec3<T> c)
            : a(a)
            , b(b)
            , c(c) { }
    };

    // Returns the barycentric coordinates of point p with respect to the triangle abc,
    // which express any position on the plane (p) as a linear combination of the three vertices positions (a, b, c)
    // obs.: the sum of the barycentric coordinates is 1, and p only lies inside abc iff all coordinates are positive
    Types::Vec3f barycentric_coords(
        const Types::Vec2i &p,
        const Triangle2D<int> &abc
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
        const Triangle2D<T> &abc
    ) {
        return coords.x * Types::Vec2<float>(abc.a) +
               coords.y * Types::Vec2<float>(abc.b) +
               coords.z * Types::Vec2<float>(abc.c); // (1-u-v) * a + u * b + v * c
    }

    template <typename T>
    Types::Vec3f barycentric_interp(
        const Types::Vec3f &coords,
        const Triangle3D<T> &abc
    ) {
        return coords.x * Types::Vec3<float>(abc.a) +
               coords.y * Types::Vec3<float>(abc.b) +
               coords.z * Types::Vec3<float>(abc.c); // (1-u-v) * a + u * b + v * c
    };
}

#endif // __GEOMETRY_HH__