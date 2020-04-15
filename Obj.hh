#ifndef __OBJ_HH__
#define __OBJ_HH__

#include <string>
#include <vector>

#include "tgaimage.hh"

#include "Types.hh"
#include "Primitives.hh"

namespace Obj {

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
    /// FaceIndices ///////////////////////////////////////
    ///////////////////////////////////////////////////////

    struct FaceIndices {
        VertexIndices v0;
        VertexIndices v1;
        VertexIndices v2;
        // TODO use a std::vector<VertexIndices> instead (and treat non-triangular faces)

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

            // ref.: https://help.poliigon.com/en/articles/1712652-what-are-the-different-texture-maps-for
            TGAImage _diffuse_map;  // color
            TGAImage _normal_map;   // bump
            TGAImage _specular_map; // reflection

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

            Primitives::Face face(int i); // builds a face from _faces_indices[i]

            Primitives::Vertex vertex(VertexIndices i);
            Primitives::Vertex vertex(int iface, int nthvert); // vertex(face_indices(iface)[nthvert])

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

            TGAColor diffuse_map_at(Types::Vec2f uv) const;
            Types::Vec3f normal_map_at(Types::Vec2f uv) const;
            float specular_map_at(Types::Vec2f uv) const;
    };
}

#endif // __OBJ_HH__