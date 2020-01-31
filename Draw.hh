#ifndef __DRAW_HH__
#define __DRAW_HH__

#include <algorithm>

#include "tgaimage.hh"

#include "Types.hh"

namespace Draw {

    void point(Types::Vec2i at,
               TGAImage &image, const TGAColor &color) {
        image.set(at.x, at.y, color);
    }

    void line(Types::Vec2i from, Types::Vec2i to,
              TGAImage &image, const TGAColor &color) {
        // make dx positive (so we draw from left to right)
        if (from.x > to.x)
            std::swap(from, to);

        int dx = to.x - from.x;
        int dy = to.y - from.y;
        int error2 = 0; // use 2 * error * dx (or dy) so we can avoid floats

        if (dx < std::abs(dy)) {
            int derror2 = std::abs(dx) * 2; // |dx| < |dy|
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
            int derror2 = std::abs(dy) * 2; // |dx| >= |dy|
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
}

#endif // __DRAW_HH__