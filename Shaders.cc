#include "Shaders.hh"

#include "Geometry.hh"

using Types::Vec2f;
using Types::Vec3f;
using Types::Vec4f;
using Types::Mat4f;
using Types::Mat3f;

namespace Shaders {

    ///////////////////////////////////////////////////////
    /// Flat Shader ///////////////////////////////////////
    ///////////////////////////////////////////////////////

    Types::Vec3f Flat::vertex(int iface, int nthvert) {
        varying_normal[nthvert] = (
           uniform_mvp_inv_T * Vec4f(uniform_model->normal(iface, nthvert), 0)
        ).xyz();

        varying_intensity[nthvert] = std::max(0.0f, dot(varying_normal[nthvert], uniform_light_direction));

        // convert object space to screen space
        Vec4f screen_coord = uniform_viewport * (
            uniform_mvp * Vec4f(uniform_model->position(iface, nthvert), 1)
        ).homogenize();
        assert(screen_coord.w == 1);
        return screen_coord.xyz();
    }

    bool Flat::fragment(Types::Vec3f frag_coord, TGAColor &frag_color) {
        // interpolate values for the current pixel
        Vec3f normal = Vec3f(
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_normal[0].x, varying_normal[1].x, varying_normal[2].x)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_normal[0].y, varying_normal[1].y, varying_normal[2].y)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_normal[0].z, varying_normal[1].z, varying_normal[2].z))
        ).normalize();

        float intensity = std::max(0.0f, dot(normal, uniform_light_direction)); // the light is behind when values are negative

        frag_color = uniform_color * intensity;

        return false; // signal that we won't discard this pixel
    }

    ///////////////////////////////////////////////////////
    /// Gouraud Shader ////////////////////////////////////
    ///////////////////////////////////////////////////////

    Types::Vec3f Gouraud::vertex(int iface, int nthvert) {
        varying_uv[nthvert] = uniform_model->uv(iface, nthvert);
        varying_normal[nthvert] = (
           uniform_mvp_inv_T * Vec4f(uniform_model->normal(iface, nthvert), 0)
        ).xyz();

        varying_intensity[nthvert] = std::max(0.0f, dot(varying_normal[nthvert], uniform_light_direction));

        // convert object space to clip space through the ModelViewProjection transform
        varying_clip_coord[nthvert] = uniform_mvp * Vec4f(uniform_model->position(iface, nthvert), 1);

        // convert clip space to NDC through perspective division
        varying_ndc[nthvert] = varying_clip_coord[nthvert].homogenized();

        // convert NDC to screen space through Viewport transform
        Vec4f screen_coord = uniform_viewport * varying_ndc[nthvert];
        assert(screen_coord.w == 1);
        return screen_coord.xyz();
    }

    bool Gouraud::fragment(Types::Vec3f frag_coord, TGAColor &frag_color) {
        // interpolate values for the current pixel
        Vec2f uv = Vec2f(
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_uv[0].x, varying_uv[1].x, varying_uv[2].x)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_uv[0].y, varying_uv[1].y, varying_uv[2].y))
        );

        float intensity = Geometry::barycentric_interp(frag_coord, varying_intensity);
        intensity = std::max(0.0f, intensity); // the light is behind when values are negative

        frag_color = uniform_model->diffuse_map_at(uv) * intensity;

        return false; // signal that we won't discard this pixel
    }

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
        varying_ndc[nthvert] = varying_clip_coord[nthvert].homogenized();

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

        // tangent space normal mapping
        Mat3f A;
        A.set_row(0, (varying_ndc[1] - varying_ndc[0]).xyz()); // B - A = AB
        A.set_row(1, (varying_ndc[2] - varying_ndc[0]).xyz()); // C - A = AC
        A.set_row(2, normal);

        // compute the Darboux basis (i, j, normal)
        Mat3f A_inv = A.inversed();
        Vec3f i = A_inv * Vec3f(varying_uv[1].x - varying_uv[0].x, // B.u - A.u
                                varying_uv[2].x - varying_uv[0].x, // C.u - A.u
                                0).normalize();
        Vec3f j = A_inv * Vec3f(varying_uv[1].y - varying_uv[0].y, // B.v - A.v
                                varying_uv[2].y - varying_uv[0].y, // C.v - A.v
                                0).normalize();
        Mat3f B; // tangent basis
        B.set_col(0, i);
        B.set_col(0, j);
        B.set_col(0, normal);

        // change from tanget basis to global coordinates
        Vec3f n = (B * uniform_model->normal_map_at(uv)).normalize();

        float intensity = std::max(0.0f, dot(n, uniform_light_direction)); // the light is behind when values are negative
        frag_color = uniform_model->diffuse_map_at(uv) * intensity;

        return false; // signal that we won't discard this pixel
    }

    ///////////////////////////////////////////////////////
    /// Phong Shader //////////////////////////////////////
    ///////////////////////////////////////////////////////

    Types::Vec3f Phong::vertex(int iface, int nthvert) {
        varying_uv[nthvert] = uniform_model->uv(iface, nthvert);
        varying_normal[nthvert] = (
           uniform_mvp_inv_T * Vec4f(uniform_model->normal(iface, nthvert), 0)
        ).xyz();

        // convert object space to clip space through the ModelViewProjection transform
        varying_clip_coord[nthvert] = uniform_mvp * Vec4f(uniform_model->position(iface, nthvert), 1);

        // convert clip space to NDC through perspective division
        varying_ndc[nthvert] = varying_clip_coord[nthvert].homogenized();

        // convert NDC to screen space through Viewport transform
        Vec4f screen_coord = uniform_viewport * varying_ndc[nthvert];
        assert(screen_coord.w == 1);
        return screen_coord.xyz();
    }

    bool Phong::fragment(Types::Vec3f frag_coord, TGAColor &frag_color) {
        // interpolate values for the current pixel
        Vec2f uv = Vec2f(
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_uv[0].x, varying_uv[1].x, varying_uv[2].x)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_uv[0].y, varying_uv[1].y, varying_uv[2].y))
        );

        Vec3f normal = (
           uniform_mvp_inv_T * Vec4f(uniform_model->normal_map_at(uv), 0)
        ).xyz().normalize();

        Vec3f light_dir = uniform_light_direction.normalized(); // make sure it's normalized

        float intensity = dot(normal, light_dir);
        Vec3f reflected_light_dir = (2 * intensity * normal - light_dir).normalize();

        float spec = std::pow(std::max(0.0f, reflected_light_dir.z),
                              uniform_model->specular_map_at(uv)); // material shininess
        float diff = std::max(0.0f, intensity); // the light is behind when values are negative

        TGAColor color = uniform_model->diffuse_map_at(uv);
        frag_color = color;
        for (int i = 0; i < 3; i++) {
            frag_color[i] = std::min(
                uniform_ka + color[i] * (uniform_kd * diff + uniform_ks * spec),
                255.0f
            );
        }

        return false; // signal that we won't discard this pixel
    }

    ///////////////////////////////////////////////////////
    /// Depth Shader //////////////////////////////////////
    ///////////////////////////////////////////////////////

    Types::Vec3f Depth::vertex(int iface, int nthvert) {
        // convert object space to clip space through the ModelViewProjection transform
        varying_clip_coord[nthvert] = uniform_mvp * Vec4f(uniform_model->position(iface, nthvert), 1);

        // convert clip space to NDC through perspective division
        varying_ndc[nthvert] = varying_clip_coord[nthvert].homogenized();

        // convert NDC to screen space through Viewport transform
        Vec4f screen_coord = uniform_viewport * varying_ndc[nthvert];
        assert(screen_coord.w == 1);
        return screen_coord.xyz();
    }

    bool Depth::fragment(Types::Vec3f frag_coord, TGAColor &frag_color) {
        // interpolate values for the current pixel
        Vec3f ndc = Vec3f(
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_ndc[0].x, varying_ndc[1].x, varying_ndc[2].x)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_ndc[0].y, varying_ndc[1].y, varying_ndc[2].y)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_ndc[0].z, varying_ndc[1].z, varying_ndc[2].z))
        );

        frag_color = TGAColor(255, 255, 255) * (ndc.z / uniform_depth_range);

        return false; // signal that we won't discard this pixel
    }
}