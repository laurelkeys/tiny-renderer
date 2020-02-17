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

///////////////////////////////////////////////////////
/// scene /////////////////////////////////////////////
///////////////////////////////////////////////////////

Obj::Model *model = nullptr;
const Vec2i resolution(800, 800);
const Mat4f viewport = Transform::viewport(resolution.x, resolution.y);

const Vec3f light_direction = Vec3f(1, 1, 0).normalize();
const Vec3f up(0, 1, 0);
const Vec3f eye(1, 1, 4);
const Vec3f center(0, 0, 0);

const Mat4f model_view = Transform::look_at(eye, center, up);
const Mat4f projection = Transform::projection((eye - center).length());
// const Mat4f projection = Transform::perspective(90, resolution.x / resolution.y, 0, 255);

///////////////////////////////////////////////////////
/// shaders ///////////////////////////////////////////
///////////////////////////////////////////////////////

struct ShaderImpl : public Shader {
    // uniforms are constant values passed to the shader
    Mat4f uniform_mvp; // ModelViewProjection matrix
    Mat4f uniform_mvp_inv_T; // (MVP^-1)^T, obs.: (A^-1)^T === (A^T)^-1

    // varyings are written by vertex shader, read by fragment shader
    Vec2f varying_uv[3];

    virtual Vec4f vertex(int iface, int nthvert) override {
        varying_uv[nthvert] = model->uv(iface, nthvert);

        // read the vertex position from the .obj model file
        Vec4f vertex_coords = Vec4f(model->position(iface, nthvert), 1); // [-1, 1]

        // transform NDC to screen coordinates
        return viewport * projection * model_view * vertex_coords;
    }

    virtual bool fragment(Vec3f bary, TGAColor &color) override {
        // interpolate uv for the current pixel
        Vec3f vertices_u_coords = Vec3f(varying_uv[0].x, varying_uv[1].x, varying_uv[2].x);
        Vec3f vertices_v_coords = Vec3f(varying_uv[0].y, varying_uv[1].y, varying_uv[2].y);
        Vec2f uv = Vec2f(
            Geometry::barycentric_interp(bary, vertices_u_coords),
            Geometry::barycentric_interp(bary, vertices_v_coords)
        );

        // after a transformation by an affine mapping, normal vectors must be transformed by a mapping
        // equal to the transpose of the inverse of the original mapping matrix (so they remain normal)
        // ref.: http://www.songho.ca/opengl/gl_normaltransform.html
        Vec3f n = (
            uniform_mvp_inv_T * Vec4f(model->normal_map_at(uv), 1)
        ).xyz().normalize();

        Vec3f l = (
            uniform_mvp * Vec4f(light_direction, 1)
        ).xyz().normalize();
        l.z *= -1; // FIXME this is a workaround for some error in the MVP matrix

        float intensity = dot(n, l);
        Vec3f r = ((2 * intensity * n) - l).normalize(); // reflected light

        float amb = 5;
        float diff = std::max(0.0f, intensity); // the light is behind when values are negative
        float spec = std::pow(std::max(0.0f, r.z), model->specular_map_at(uv));

        TGAColor c = model->diffuse_map_at(uv);
        for (int i = 0; i < 3; ++i) {
            color[i] = Math::clamp(
                amb + c[i] * (diff + 0.6f * spec),
                0.0f, 255.0f
            );
        }

        // return false to signal that we won't discard this pixel
        return false;
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

    ShaderImpl shader;
    Mat4f mvp = projection * model_view;
    shader.uniform_mvp = mvp;
    shader.uniform_mvp_inv_T = mvp.inversed().transposed();

    for (int i = 0; i < model->n_of_faces(); ++i) {
        Vec3i screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            // FIXME make screen_coords Vec4f instead
            screen_coords[j] = Vec3i((
                shader.vertex(i, j)
            ).homogenize().xyz());
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