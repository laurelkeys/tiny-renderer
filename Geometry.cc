#include "Geometry.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

namespace Geometry {

    Vec3f barycentric_coords(const Vec2i &p, const Triangle2D<int> &abc) {
        Vec2i AB = abc.b - abc.a;
        Vec2i AC = abc.c - abc.a;
        Vec2i PA = abc.a - p;

        // given a triangle abc and a point p, defined by:
        //   p = (1-u-v) * a + u * b + v * c = a + u * AB + v * AC,
        // p is inside the triangle iff:
        //   (0 <= u, v <= 1) and (u + v <= 1)

        Vec3i coords = cross(Vec3i(AC.x, AB.x, PA.x),
                             Vec3i(AC.y, AB.y, PA.y));

        if (coords.z == 0) {
            // the triangle is degenerate, so we return a negative coordinate
            // for it to be skipped (as it means p lies outside triangle abc)
            return Vec3f(-1, -1, -1); // obs.: coords.z is proportional to the area of abc
        }

        return Vec3f(
            1 - (coords.x + coords.y) / static_cast<float>(coords.z), // 1 - u - v
            coords.y / static_cast<float>(coords.z), // u = (PA.x * AC.y - AC.x * PA.y) / (AC.x * AB.y - AB.x * AC.y)
            coords.x / static_cast<float>(coords.z)  // v = (AB.x * PA.y - AB.y * PA.x) / (AC.x * AB.y - AB.x * AC.y)
        );
    }

    Vec2f barycentric_interp(const Vec3f &coords, const Triangle2D<int> &abc) {
        return coords.x * Vec2f(abc.a) +
               coords.y * Vec2f(abc.b) +
               coords.z * Vec2f(abc.c); // (1-u-v) * a + u * b + v * c
    }

    Vec3f barycentric_interp(const Vec3f &coords, const Triangle3D<int> &abc) {
        return coords.x * Vec3f(abc.a) +
               coords.y * Vec3f(abc.b) +
               coords.z * Vec3f(abc.c); // (1-u-v) * a + u * b + v * c
    }
}