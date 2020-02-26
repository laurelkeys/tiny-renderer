#ifndef __GEOMETRY_HH__
#define __GEOMETRY_HH__

#include "Math.hh"
#include "Types.hh"

namespace Geometry {

    // obs.: the barycentric coordinates represent the point as a
    //       linear combination of the triangle's vertices positions
    struct PointProps {
        bool is_inside_triangle; // true iff all barycentric_coords are positive
        Types::Vec3f barycentric_coords; // (1-u-v, u, v)
    };

    template <typename T>
    struct TriangleXY {
        Types::Vec2<T> a, b, c;

        TriangleXY(Types::Vec2<T> a, Types::Vec2<T> b, Types::Vec2<T> c)
            : a(a)
            , b(b)
            , c(c) { }

        TriangleXY(Types::Vec3<T> a, Types::Vec3<T> b, Types::Vec3<T> c)
            : a(a.xy())
            , b(b.xy())
            , c(c.xy()) { }
    };

    ///////////////////////////////////////////////////////
    /// barycentric coordinates ///////////////////////////
    ///////////////////////////////////////////////////////

    PointProps barycentric_coords(
        const Types::Vec2i &p,
        const TriangleXY<int> &triangle
    );

    ///////////////////////////////////////////////////////
    /// barycentric interpolation /////////////////////////
    ///////////////////////////////////////////////////////

    template <typename T>
    float barycentric_interp(
        const Types::Vec3f &coords,
        const T &a, const T &b, const T &c
    ) {
        return coords.x * static_cast<float>(a) +
               coords.y * static_cast<float>(b) +
               coords.z * static_cast<float>(c); // (1-u-v) * a + u * b + v * c
    }
    template <typename T>
    float barycentric_interp(const Types::Vec3f &coords, const Types::Vec3<T> &abc) {
        return barycentric_interp(coords, abc.x, abc.y, abc.z);
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

    template <typename T>
    Types::Vec4f barycentric_interp(
        const Types::Vec3f &coords,
        const Types::Vec4<T> &a, const Types::Vec4<T> &b, const Types::Vec4<T> &c
    ) {
        return coords.x * Types::Vec4<float>(a) +
               coords.y * Types::Vec4<float>(b) +
               coords.z * Types::Vec4<float>(c); // (1-u-v) * a + u * b + v * c
    };
}

#endif // __GEOMETRY_HH__