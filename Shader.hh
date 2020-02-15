#ifndef __SHADER_HH__
#define __SHADER_HH__

#include "tgaimage.hh"

#include "Types.hh"

struct Shader {
    virtual ~Shader();
    virtual Types::Vec4f vertex(int iface, int nthvert) = 0;
    virtual bool fragment(Types::Vec3f bary, TGAColor &color) = 0;
};

#endif // __SHADER_HH__