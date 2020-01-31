#ifndef __MODEL_H__
#define __MODEL_H__

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tgaimage.hh"

#include "Types.hh"

class Model {
    private:
        std::vector<Types::Vec3f> verts_;
        std::vector<std::vector<Types::Vec3i>> faces_; // attention, this Vec3i means vertex/uv/normal
        std::vector<Types::Vec3f> norms_;
        std::vector<Types::Vec2f> uv_;
        TGAImage diffusemap_;
        TGAImage normalmap_;
        TGAImage specularmap_;
        void load_texture(std::string filename, const char *suffix, TGAImage &img);

    public:
        Model(const char *filename);
        ~Model();
        int nverts();
        int nfaces();
        Types::Vec3f normal(int iface, int nthvert);
        Types::Vec3f normal(Types::Vec2f uv);
        Types::Vec3f vert(int i);
        Types::Vec3f vert(int iface, int nthvert);
        Types::Vec2f uv(int iface, int nthvert);
        TGAColor diffuse(Types::Vec2f uv);
        float specular(Types::Vec2f uv);
        std::vector<int> face(int idx);
};
#endif //__MODEL_H__