#include "Geometry.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

namespace Geometry {

    PointProps barycentric_coords(const Vec2i &p, const Vec2i &a, const Vec2i &b, const Vec2i &c) {
        Vec2i AB = b - a;
        Vec2i AC = c - a;
        Vec2i PA = a - p;

        // given a triangle abc and a point p, defined by:
        //   p = (1-u-v) * a + u * b + v * c = a + u * AB + v * AC,
        // p is inside the triangle iff:
        //   (0 <= u, v <= 1) and (u + v <= 1)

        Vec3i coords = cross(Vec3i(AC.x, AB.x, PA.x),
                             Vec3i(AC.y, AB.y, PA.y));

        float area2 = static_cast<float>(coords.z); // 2 * area(a, b, c)

        PointProps point;

        if (std::abs(area2) <= Math::EPS_FLOAT) {
            // the triangle is degenerate (area == 0), so we determine that
            // the point p lies outside the triangle abc for it to be skipped
            point.is_inside_triangle = false;
            return point;
        }

        point.barycentric_coords = Vec3f(
            1 - (coords.y + coords.x) / area2, // 1 - u - v
            coords.y / area2, // u = (PA.x * AC.y - AC.x * PA.y) / (AC.x * AB.y - AB.x * AC.y)
            coords.x / area2  // v = (AB.x * PA.y - AB.y * PA.x) / (AC.x * AB.y - AB.x * AC.y)
        );

        point.is_inside_triangle = point.barycentric_coords.x >= 0 &&
                                   point.barycentric_coords.y >= 0 &&
                                   point.barycentric_coords.z >= 0;
        return point;
    }
}