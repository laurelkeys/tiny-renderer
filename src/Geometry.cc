#include "Geometry.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

namespace Geometry {

    PointProps barycentric_coords(const Vec2i &p, const TriangleXY<int> &triangle) {
        Vec2i AB = triangle.b - triangle.a;
        Vec2i AC = triangle.c - triangle.a;
        Vec2i AP = p - triangle.a;

        // given a triangle ABC and a point P, defined by:
        //   P = (1-u-v) * A + u * B + v * C = A + u * AB + v * AC,
        // P is inside the triangle iff:
        //   (0 <= u, v <= 1) and (u + v <= 1)

        float area2 = AB.x * AC.y - AB.y * AC.x;

        PointProps point;
        if (std::abs(area2) <= Math::EPS_FLOAT) {
            // the triangle is degenerate (area == 0), so we determine that
            // the point P lies outside the triangle ABC for it to be skipped
            point.is_inside_triangle = false;
            return point;
        }

        float u = (AC.y * AP.x - AC.x * AP.y) / area2;
        float v = (AB.x * AP.y - AB.y * AP.x) / area2;

        point.barycentric_coords = Vec3f(1 - (u + v), u, v);
        point.is_inside_triangle = point.barycentric_coords.x >= 0 &&
                                   point.barycentric_coords.y >= 0 &&
                                   point.barycentric_coords.z >= 0;
        return point;
    }
}