#ifndef __SHADERS_HH__
#define __SHADERS_HH__

#include "tgaimage.hh"

#include "Obj.hh"
#include "Types.hh"
#include "Shader.hh"

namespace Shaders {

    // uniforms are constant values passed to the shader
    // varyings are written by the vertex shader, and read by the fragment shader

    ///////////////////////////////////////////////////////
    /// Texture Shader ////////////////////////////////////
    ///////////////////////////////////////////////////////

    struct Texture : public Shader {

        Types::Vec3f vertex(int iface, int nthvert) override;
        bool fragment(Types::Vec3f frag_coord, TGAColor &frag_color) override;

        /// uniforms //////////////////////////////////////////

        Types::Mat4f uniform_mvp;
        Types::Mat4f uniform_mvp_inv_T;
        Types::Mat4f uniform_viewport;

        Types::Vec3f uniform_light_direction;
        Obj::Model *uniform_model;

        /// varyings //////////////////////////////////////////

        Types::Vec2f varying_uv[3];
        Types::Vec3f varying_intensity; // float varying_intensity[3]
        Types::Vec3f varying_normal[3];
        Types::Vec4f varying_clip_coord[3];
        Types::Vec4f varying_ndc[3];
    };
}

#endif // __SHADERS_HH__