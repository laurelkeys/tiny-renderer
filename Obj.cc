#include "Obj.hh"

#include <sstream>

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

namespace Obj {

    ///////////////////////////////////////////////////////
    /// FaceIndices ///////////////////////////////////////
    ///////////////////////////////////////////////////////

    VertexIndices &FaceIndices::operator[](unsigned int i) {
        switch (i) {
            case 0: return v0;
            case 1: return v1;
            case 2: return v2;
            default: // invalid index
                assert(i < 3);
                return v0;
        }
    }

    const VertexIndices &FaceIndices::operator[](unsigned int i) const {
        switch (i) {
            case 0: return v0;
            case 1: return v1;
            case 2: return v2;
            default: // invalid index
                assert(i < 3);
                return v0;
        }
    }

    ///////////////////////////////////////////////////////
    /// Model /////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    Model::Model(const char *filename)
        : _positions()
        , _uv_textures()
        , _normals()
        , _faces_indices()
        , _diffuse_map()
        , _normal_map()
        , _specular_map() {
        std::ifstream in;
        in.open(filename, std::ifstream::in);
        if (in.fail())
            return;

        std::string line;
        while (!in.eof()) {
            std::getline(in, line);
            std::istringstream iss(line.c_str());
            char trash;

            if (!line.compare(0, 2, "v ")) {
                iss >> trash;
                Vec3f p;
                for (int i = 0; i < 3; i++)
                    iss >> p[i];
                _positions.push_back(p);
            } else if (!line.compare(0, 3, "vt ")) {
                iss >> trash >> trash;
                Vec2f t;
                for (int i = 0; i < 2; i++)
                    iss >> t[i];
                _uv_textures.push_back(t);
            } else if (!line.compare(0, 3, "vn ")) {
                iss >> trash >> trash;
                Vec3f n;
                for (int i = 0; i < 3; i++)
                    iss >> n[i];
                _normals.push_back(n);
            } else if (!line.compare(0, 2, "f ")) {
                iss >> trash;
                FaceIndices face_indices;
                int nth = 0;
                int p, t, n; // vertex indices in _positions, _uv_textures and _normals
                while (iss >> p >> trash >> t >> trash >> n) {
                    assert(nth < 3);
                    // in wavefront obj all indices start at 1, not zero
                    face_indices[nth++] = VertexIndices(p - 1, t - 1, n - 1);
                }
                assert(nth == 3); // triangular faces
                _faces_indices.push_back(face_indices);
            }
        }

        std::cerr << "# v# " << _positions.size()
                  << " vt# " << _uv_textures.size()
                  << " vn# " << _normals.size()
                  << " f# "  << _faces_indices.size()
                  << std::endl;

        load_texture(filename, "_diffuse.tga", _diffuse_map);
        load_texture(filename, "_nm_tangent.tga", _normal_map);
        load_texture(filename, "_spec.tga", _specular_map);
    }

    Model::~Model() { }

    void Model::load_texture(std::string filename, const char *suffix, TGAImage &image) {
        std::string texfile(filename);
        size_t dot = texfile.find_last_of(".");
        if (dot != std::string::npos) {
            texfile = texfile.substr(0, dot) + std::string(suffix);
            std::cerr << "texture file " << texfile << " loading "
                      << (image.read_tga_file(texfile.c_str()) ? "ok" : "failed")
                      << std::endl;
            image.flip_vertically();
        }
    }

    int Model::n_of_vertices() {
        return static_cast<int>(_positions.size());
    }

    int Model::n_of_faces() {
        return static_cast<int>(_faces_indices.size());
    }

    Primitives::Face Model::face(int i) {
        FaceIndices face_indices = _faces_indices[i];
        return Primitives::Face(
            vertex(face_indices.v0),
            vertex(face_indices.v1),
            vertex(face_indices.v2)
        );
    }

    Primitives::Vertex Model::vertex(VertexIndices i) {
        return Primitives::Vertex(
            _positions[i.p],
            _uv_textures[i.t],
            _normals[i.n]
        );
    }

    Primitives::Vertex Model::vertex(int iface, int nthvert) {
        VertexIndices i = _faces_indices[iface][nthvert];
        return vertex(i);
    }

    FaceIndices Model::face_indices(int i) {
        return _faces_indices[i];
    }

    Vec3f Model::position(int i) {
        return _positions[i];
    }

    Vec3f Model::position(int iface, int nthvert) {
        return position(_faces_indices[iface][nthvert].p);
    }

    Vec2f Model::uv(int i) {
        return _uv_textures[i];
    }

    Vec2f Model::uv(int iface, int nthvert) {
        return uv(_faces_indices[iface][nthvert].t);
    }

    Vec3f Model::normal(int i) {
        return _normals[i];
    }

    Vec3f Model::normal(int iface, int nthvert) {
        return normal(_faces_indices[iface][nthvert].n);
    }

    TGAColor Model::diffuse_map_at(Vec2f uv) const {
        Vec2i UV(
            uv.x * _diffuse_map.get_width(), // map [-1, 1] to [-width, width]
            uv.y * _diffuse_map.get_height() // map [-1, 1] to [-height, height]
        );
        return _diffuse_map.get(UV.x, UV.y);
    }

    Vec3f Model::normal_map_at(Vec2f uv) const {
        Vec2i UV(
            uv.x * _normal_map.get_width(), // map [-1, 1] to [-width, width]
            uv.y * _normal_map.get_height() // map [-1, 1] to [-height, height]
        );
        TGAColor c = _normal_map.get(UV.x, UV.y); // BGRA
        return Vec3f(
            static_cast<float>(c[2]) / 255.f * 2.f - 1.f, // R
            static_cast<float>(c[1]) / 255.f * 2.f - 1.f, // G
            static_cast<float>(c[0]) / 255.f * 2.f - 1.f  // B
        ); // map [0, 255] to [-1, 1]
    }

    float Model::specular_map_at(Vec2f uv) const {
        Vec2i UV(
            uv.x * _specular_map.get_width(), // map [-1, 1] to [-width, width]
            uv.y * _specular_map.get_height() // map [-1, 1] to [-height, height]
        );
        return static_cast<float>(_specular_map.get(UV.x, UV.y)[0]); // B channel
    }
}