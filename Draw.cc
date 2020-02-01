#include <algorithm>

#include "Draw.hh"
#include "Geometry.hh"

using Types::Vec2i;
using Types::Vec3f;

namespace Draw {

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

    void triangle(Vec2i a, Vec2i b, Vec2i c,
                  TGAImage &image, const TGAColor &color) {
        const Vec2i bbox_min(std::max(0, std::min(std::min(a.x, b.x), c.x)),
                             std::max(0, std::min(std::min(a.y, b.y), c.y)));
        const Vec2i bbox_max(std::min(image.get_width() - 1, std::max(std::max(a.x, b.x), c.x)),
                             std::min(image.get_height() - 1, std::max(std::max(a.y, b.y), c.y)));

        Vec2i p;
        for (p.x = bbox_min.x; p.x <= bbox_max.x; ++p.x) {
            for (p.y = bbox_min.y; p.y <= bbox_max.y; ++p.y) {
                Vec3f bc_screen = Geometry::barycentric_coords(p, Geometry::Triangle<int>(a, b, c));

                if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                    continue; // point lies outside the triangle

                image.set(p.x, p.y, color);
            }
        }
    }
}