#ifndef __DRAW_HH__
#define __DRAW_HH__

#include "tgaimage.hh"

#include "Obj.hh"
#include "Types.hh"
#include "Shader.hh"

namespace Draw {

    template <typename T>
    struct TriangleProps {
        T a, b, c;

        TriangleProps(T props[3])
            : a(props[0])
            , b(props[1])
            , c(props[2]) { }
    };

    ///////////////////////////////////////////////////////
    /// 2D ////////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    void point(
        Types::Vec2i at,
        TGAImage &image, const TGAColor &color
    );

    void line(
        Types::Vec2i from, Types::Vec2i to,
        TGAImage &image, const TGAColor &color
    );

    ///////////////////////////////////////////////////////
    /// 3D (in screen space) //////////////////////////////
    ///////////////////////////////////////////////////////

    void triangle(
        TriangleProps<Types::Vec3f> pos, Shader &shader,
        float z_buffer[], TGAImage &image, Obj::Model *model
    );
}

#endif // __DRAW_HH__