#ifndef __PRIMITIVES_HH__
#define __PRIMITIVES_HH__

#include "Types.hh"

namespace Primitives {

    ///////////////////////////////////////////////////////
    /// Vertex ////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    struct Vertex {
        // obs.: using [-1.0, 1.0] instead of [0.0, 1.0] accounts for wrapping
        Types::Vec3f pos;    // v  : (x, y, z) ∈ [-1.0, 1.0], geometric position
        Types::Vec2f uv;     // vt : (u, v)    ∈ [-1.0, 1.0], UV texture coordinates
        Types::Vec3f normal; // vn : (i, j, k) ∈ [-1.0, 1.0], vertex normal

        Vertex() = delete;

        explicit Vertex(const Types::Vec3f &pos)
            : pos(pos) { }

        explicit Vertex(const Types::Vec3f &pos, const Types::Vec2f &uv)
            : pos(pos)
            , uv(uv) { }

        explicit Vertex(const Types::Vec3f &pos, const Types::Vec2f &uv, const Types::Vec3f &normal)
            : pos(pos)
            , uv(uv)
            , normal(normal) { }

        Vertex(const Vertex &vert)
            : pos(vert.pos)
            , uv(vert.uv)
            , normal(vert.normal) { }
    };    

    ///////////////////////////////////////////////////////
    /// Face //////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    // Triangular face
    struct Face {
        Vertex v0;
        Vertex v1;
        Vertex v2;

        Face() = default;

        explicit Face(const Vertex &v0, const Vertex &v1, const Vertex &v2)
            : v0(v0)
            , v1(v1)
            , v2(v2) { }

        Vertex &operator[](unsigned int i);
        const Vertex &operator[](unsigned int i) const;
    };
}

#endif // __PRIMITIVES_HH__