#include "colors.hh"
#include "tgaimage.hh"

#include "Obj.hh"
#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"
#include "Shader.hh"
#include "Geometry.hh"
#include "Transform.hh"
#include "Primitives.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

using Types::Vec4f; // homogeneous coordinates (points with w=1, vectors with w=0)
using Types::Mat4f;
using Types::Mat3f;

///////////////////////////////////////////////////////
/// scene /////////////////////////////////////////////
///////////////////////////////////////////////////////

Obj::Model *model = nullptr;
const Vec2i resolution(800, 800);
const Mat4f viewport = Transform::viewport(resolution.x, resolution.y); // depth = 255

const Vec3f up(0, 1, 0);
const Vec3f eye(0, 0, 3); // (0, 0, focal_dist)
const Vec3f center(0, 0, 0);
const Vec3f light_direction = Vec3f(1, 1, 1).normalize();

///////////////////////////////////////////////////////
/// shaders ///////////////////////////////////////////
///////////////////////////////////////////////////////

struct ShaderImpl : public Shader {
    // uniforms are constant values passed to the shader
    Mat4f uniform_mvp;
    Mat4f uniform_mvp_inv_T;

    // varyings are written by vertex shader, read by fragment shader
    Vec2f varying_uv[3];
    Vec3f varying_intensity; // float varying_intensity[3]
    Vec3f varying_normal[3];
    Vec4f varying_clip_coord[3];
    Vec4f varying_ndc[3];

    Vec3f vertex(int iface, int nthvert) override {
        varying_uv[nthvert] = model->uv(iface, nthvert);
        varying_normal[nthvert] = model->normal(iface, nthvert);

        varying_intensity[nthvert] = std::max(0.0f, dot(varying_normal[nthvert], light_direction));

        // convert object space to clip space through the ModelViewProjection transform
        varying_clip_coord[nthvert] = uniform_mvp * Vec4f(model->position(iface, nthvert), 1);

        // convert clip space to NDC through perspective division
        varying_ndc[nthvert] = varying_clip_coord[nthvert].homogenize();

        // convert NDC to screen space through Viewport transform
        Vec4f screen_coord = viewport * varying_ndc[nthvert];
        assert(screen_coord.w == 1);
        return screen_coord.xyz();
    }

    bool fragment(Vec3f frag_coord, TGAColor &frag_color) override {
        // interpolate values for the current pixel
        Vec2f uv = Vec2f(
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_uv[0].x, varying_uv[1].x, varying_uv[2].x)),
            Geometry::barycentric_interp(frag_coord, Vec3f(varying_uv[0].y, varying_uv[1].y, varying_uv[2].y))
        );

        float intensity = Geometry::barycentric_interp(frag_coord, varying_intensity);
        float diff = std::max(0.0f, intensity); // the light is behind when values are negative
        frag_color = model->diffuse_map_at(uv) * diff;

        return false; // signal that we won't discard this pixel
    }
};

///////////////////////////////////////////////////////
/// main //////////////////////////////////////////////
///////////////////////////////////////////////////////

int main(int argc, char **argv) {
    model = new Obj::Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);
    float *z_buffer = new float[resolution.x * resolution.y];
    for (int i = 0; i < resolution.x * resolution.y; ++i) {
        z_buffer[i] = Math::MIN_INT;
    }

    const Mat4f model_view = Transform::look_at(eye, center, up);
    const Mat4f projection = Transform::projection((eye - center).length());
    // const Mat4f projection = Transform::perspective(90, resolution.x / resolution.y, 0, 255);

    ShaderImpl shader;
    Mat4f mvp = projection * model_view;
    shader.uniform_mvp = mvp;
    shader.uniform_mvp_inv_T = mvp.inversed().transposed();

    for (int i = 0; i < model->n_of_faces(); ++i) {
        Vec3f screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            screen_coords[j] = shader.vertex(i, j);
        }

        Draw::triangle(
            screen_coords, shader,
            z_buffer, image, model
        );
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    delete model;
    delete[] z_buffer;
    return 0;
}