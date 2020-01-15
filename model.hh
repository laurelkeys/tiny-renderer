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
        std::vector<Vec3f> verts_;
        std::vector<std::vector<Vec3i>> faces_; // { vertex, uv, normal }
        std::vector<Vec3f> norms_;
        std::vector<Vec2f> uv_;
        TGAImage diffuse_map_;
        // TGAImage normal_map_;
        // TGAImage specular_map_;

        void load_texture(std::string filename,
                          const char *suffix,
                          TGAImage &image);

    public:
        Model(const char *filename);
        ~Model();

        int nverts();
        int nfaces();
        
        Vec3f vert(int i);
        // Vec3f vert(int iface, int nvert);

        // Vec3f normal(int iface, int nvert);
        // Vec3f normal(Vec2f uv);
        
        Vec2i uv(int iface, int nvert);
        
        TGAColor diffuse(Vec2i uv);
        std::vector<int> face(int idx);
};

#endif //__MODEL_HH__