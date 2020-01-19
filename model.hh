#ifndef __MODEL_HH__
#define __MODEL_HH__

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "geometry.hh"
#include "tgaimage.hh"

class Model {
    private:
        std::vector<Vec3f> pos_;    // v  : (x, y, z) ∈ [0.0, 1.0], geometric vertices
        std::vector<Vec2f> uv_;     // vt : (u, v)    ∈ [0.0, 1.0], texture coordinates
        std::vector<Vec3f> normal_; // vn : (i, j, k) ∈ [0.0, 1.0], vertex normals
        std::vector<std::vector<Vec3i>> faces_; // f : v/vt/vn
        TGAImage diffuse_map_;
        // TGAImage normal_map_;
        // TGAImage specular_map_;

        void load_texture(std::string filename,
                          const char *suffix,
                          TGAImage &image);

    public:
        Model(const char *filename);
        ~Model();

        int n_of_vertices();
        int n_of_faces();
        
        Vec3f vertex(int i);
        Vec3f vertex(int iface, int nthvert);
        
        Vec2f uv(int iface, int nthvert);

        // Vec3f normal(Vec2f uv);
        Vec3f normal(int iface, int nthvert);
        
        std::vector<int> face(int idx);
        
        TGAColor diffuse(Vec2f uv);
        // float specular(Vec2f uv);
};

#endif //__MODEL_HH__