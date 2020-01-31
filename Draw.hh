#ifndef __DRAW_HH__
#define __DRAW_HH__

#include "tgaimage.hh"
#include "Types.hh"

namespace Draw {

    void point(Types::Vec2i at,
               TGAImage &image, const TGAColor &color);

    void line(Types::Vec2i from, Types::Vec2i to,
              TGAImage &image, const TGAColor &color);
}

#endif // __DRAW_HH__