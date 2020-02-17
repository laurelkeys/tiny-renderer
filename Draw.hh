#ifndef __DRAW_HH__
#define __DRAW_HH__

#include "tgaimage.hh"

#include "Obj.hh"
#include "Types.hh"
#include "Shader.hh"

namespace Draw {

    void point(
        Types::Vec2i at,
        TGAImage &image, const TGAColor &color
    );

    void line(
        Types::Vec2i from, Types::Vec2i to,
        TGAImage &image, const TGAColor &color
    );

    template <typename T>
    struct TriangleProps {
        T a, b, c;
        TriangleProps(T props[3])
            : a(props[0])
            , b(props[1])
            , c(props[2]) { }
    };

    void triangle(
        TriangleProps<Types::Vec3i> pos, Shader &shader,
        float z_buffer[], TGAImage &image, Obj::Model *model
    );
}

#endif // __DRAW_HH__