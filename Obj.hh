#ifndef __OBJ_HH__
#define __OBJ_HH__

#include <string>
#include <vector>

#include "tgaimage.hh"

#include "Types.hh"

namespace Obj {

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
    /// VertexIndices /////////////////////////////////////
    ///////////////////////////////////////////////////////

    struct VertexIndices {
        int p; // geometric position
        int t; // UV texture coordinates
        int n; // vertex normal

        VertexIndices() = default;

        explicit VertexIndices(int p)
            : p(p) { }

        explicit VertexIndices(int p, int t)
            : p(p)
            , t(t) { }

        explicit VertexIndices(int p, int t, int n)
            : p(p)
            , t(t)
            , n(n) { }
    };

    ///////////////////////////////////////////////////////
    /// Face //////////////////////////////////////////////
    ///////////////////////////////////////////////////////

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

    ///////////////////////////////////////////////////////
    /// FaceIndices ///////////////////////////////////////
    ///////////////////////////////////////////////////////

    struct FaceIndices {
        VertexIndices v0;
        VertexIndices v1;
        VertexIndices v2;

        FaceIndices() = default;

        explicit FaceIndices(const VertexIndices &v0, const VertexIndices &v1, const VertexIndices &v2)
            : v0(v0)
            , v1(v1)
            , v2(v2) { }

        VertexIndices &operator[](unsigned int i);
        const VertexIndices &operator[](unsigned int i) const;
    };

    ///////////////////////////////////////////////////////
    /// Model /////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    class Model {
        private:
            std::vector<Types::Vec3f> _positions;
            std::vector<Types::Vec2f> _uv_textures;
            std::vector<Types::Vec3f> _normals; // obs.: normals might not be unit vectors

            std::vector<FaceIndices> _faces_indices;

            TGAImage _diffuse_map;
            // TGAImage _normal_map;
            // TGAImage _specular_map;

            void load_texture(
                std::string filename, const char *suffix,
                TGAImage &image
            );

        public:
            Model(const char *filename);
            ~Model();

            int n_of_vertices();
            int n_of_faces();

            /// property accessors ////////////////////////////////

            Face face(int i); // builds a face from _faces_indices

            Vertex vertex(VertexIndices i);
            Vertex vertex(int iface, int nthvert);

            // below are some leaky abstractions,
            // you should favor using the ones above

            FaceIndices face_indices(int i);

            Types::Vec3f position(int i);
            Types::Vec3f position(int iface, int nthvert);

            Types::Vec2f uv(int i);
            Types::Vec2f uv(int iface, int nthvert);

            Types::Vec3f normal(int i);
            Types::Vec3f normal(int iface, int nthvert);

            /// texture maps //////////////////////////////////////

            TGAColor diffuse_map(Types::Vec2f uv);
            // Types::Vec3f normal_map(Types::Vec2f uv);
            // float specular_map(Types::Vec2f uv);
    };
}

#endif // __OBJ_HH__