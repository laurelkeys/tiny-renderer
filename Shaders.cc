#include "Shaders.hh"

#include "Geometry.hh"

using Types::Vec2f;
using Types::Vec3f;
using Types::Vec4f;
using Types::Mat4f;

namespace Shaders {

    ///////////////////////////////////////////////////////
    /// Texture Shader ////////////////////////////////////
    ///////////////////////////////////////////////////////

    Types::Vec3f Texture::vertex(int iface, int nthvert) {
        varying_uv[nthvert] = uniform_model->uv(iface, nthvert);
        varying_normal[nthvert] = (
           uniform_mvp_inv_T * Vec4f(uniform_model->normal(iface, nthvert), 0)
        ).xyz();

        varying_intensity[nthvert] = std::max(0.0f, dot(varying_normal[nthvert], uniform_light_direction));

        // convert object space to clip space through the ModelViewProjection transform
        varying_clip_coord[nthvert] = uniform_mvp * Vec4f(uniform_model->position(iface, nthvert), 1);

        // convert clip space to NDC through perspective division
        varying_ndc[nthvert] = varying_clip_coord[nthvert].homogenize();

        // convert NDC to screen space through Viewport transform
        Vec4f screen_coord = uniform_viewport * varying_ndc[nthvert];
        assert(screen_coord.w == 1);
        return screen_coord.xyz();
    }

    bool Texture::fragment(Types::Vec3f frag_coord, TGAColor &frag_color) {
        // interpolate values for the current pixel
        Vec2f uv = Vec2f(
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_uv[0].x, varying_uv[1].x, varying_uv[2].x)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_uv[0].y, varying_uv[1].y, varying_uv[2].y))
        );

        Vec3f normal = Vec3f(
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_normal[0].x, varying_normal[1].x, varying_normal[2].x)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_normal[0].y, varying_normal[1].y, varying_normal[2].y)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_normal[0].z, varying_normal[1].z, varying_normal[2].z))
        ).normalize();

        float intensity = std::max(0.0f, dot(normal, uniform_light_direction)); // the light is behind when values are negative

        frag_color = uniform_model->diffuse_map_at(uv) * intensity;

        return false; // signal that we won't discard this pixel
    }
}