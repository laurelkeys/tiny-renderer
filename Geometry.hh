#ifndef __GEOMETRY_HH__
#define __GEOMETRY_HH__

#include "Types.hh"

namespace Geometry {

    template <typename T>
    struct Triangle {
        Types::Vec2<T> a, b, c; // positions of the vertices

        Triangle() = delete;
        explicit Triangle(Types::Vec2<T> a, Types::Vec2<T> b, Types::Vec2<T> c)
            : a(a)
            , b(b)
            , c(c) { }
    };

    // Returns the barycentric coordinates of point p with respect to the triangle abc,
    // which express any position on the plane (p) as a linear combination of the three vertices positions (a, b, c)
    // obs.: the sum of the barycentric coordinates is 1, and p only lies inside abc iff all coordinates are positive
    Types::Vec3f barycentric_coords(
        const Types::Vec2i &p,
        const Triangle<int> &abc
    );

    // Uses the barycentric coordinates as interpolation coefficients
    // to perform a weighted mix of the values of the triangle vertices
    // ref.: https://fgiesen.wordpress.com/2013/02/06/the-barycentric-conspirac/
    Types::Vec2f barycentric_interp(
        const Types::Vec3f &coords,
        const Triangle<int> &abc
    );
}

#endif // __GEOMETRY_HH__