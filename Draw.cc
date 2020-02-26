#include "Draw.hh"

#include <algorithm>

#include "Math.hh"
#include "Geometry.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

namespace Draw {

    ///////////////////////////////////////////////////////
    /// 2D ////////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    void point(Vec2i at,
               TGAImage &image, const TGAColor &color) {
        image.set(at.x, at.y, color);
    }

    void line(Vec2i from, Vec2i to,
              TGAImage &image, const TGAColor &color) {
        if (std::abs(from.x - to.x) < std::abs(from.y - to.y)) {
            if (from.y > to.y)
                std::swap(from, to);
            int dx = to.x - from.x;
            int dy = to.y - from.y; // positive

            // use error*dx*2 so we can avoid floats
            int error2 = 0;
            int derror2 = std::abs(dx) * 2;

            // |dx| < |dy|, so we always increase y by 1
            // and check for when we need to increase x
            int inc_x = dx > 0 ? 1 : -1;
            int x = from.x;
            for (int y = from.y; y <= to.y; ++y) {
                image.set(x, y, color);
                error2 += derror2;
                if (error2 > dy) {
                    x += inc_x;
                    error2 -= dy * 2;
                }
            }
        } else {
            if (from.x > to.x)
                std::swap(from, to);
            int dx = to.x - from.x; // positive
            int dy = to.y - from.y;

            // use error*dy*2 so we can avoid floats
            int error2 = 0;
            int derror2 = std::abs(dy) * 2;

            // |dx| > |dy|, so we always increase x by 1
            // and check for when we need to increase y
            int inc_y = dy > 0 ? 1 : -1;
            int y = from.y;
            for (int x = from.x; x <= to.x; ++x) {
                image.set(x, y, color);
                error2 += derror2;
                if (error2 > dx) {
                    y += inc_y;
                    error2 -= dx * 2;
                }
            }
        }
    }

    ///////////////////////////////////////////////////////
    /// 3D (in screen space) //////////////////////////////
    ///////////////////////////////////////////////////////

    void triangle(TriangleProps<Types::Vec3f> pos, Shader &shader,
                  float z_buffer[], TGAImage &image, Obj::Model *model) {
        const Vec2i bbox_min = Vec2i(
            std::max(0.0f, Math::min(pos.a.x, pos.b.x, pos.c.x)),
            std::max(0.0f, Math::min(pos.a.y, pos.b.y, pos.c.y))
        ); // max({0, 0}, min(a, b, c))

        const Vec2i bbox_max = Vec2i(
            std::min(Math::max(pos.a.x, pos.b.x, pos.c.x), image.get_width() - 1.0f),
            std::min(Math::max(pos.a.y, pos.b.y, pos.c.y), image.get_height() - 1.0f)
        ); // min(max(a, b, c), {width, height})

        const Vec3f vertex_depths(pos.a.z, pos.b.z, pos.c.z);

        Vec2i p;
        for (p.x = bbox_min.x; p.x <= bbox_max.x; ++p.x) {
            for (p.y = bbox_min.y; p.y <= bbox_max.y; ++p.y) {
                Geometry::PointProps point = Geometry::barycentric_coords(p, Geometry::TriangleXY<int>(pos.a, pos.b, pos.c));

                if (!point.is_inside_triangle)
                    continue;

                // interpolate z-values with the screen coordinates of point p
                float pz = Geometry::barycentric_interp(point.barycentric_coords, vertex_depths);
                if (z_buffer[int(p.x + p.y * image.get_width())] < pz) {
                    TGAColor color;
                    bool discard = shader.fragment(point.barycentric_coords, color);
                    if (!discard) {
                        z_buffer[int(p.x + p.y * image.get_width())] = pz;
                        image.set(p.x, p.y, color);
                    }
                }
            }
        }
    }
}