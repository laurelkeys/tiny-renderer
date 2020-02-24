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
const Mat4f viewport = Transform::viewport(resolution.x, resolution.y);

const Vec3f light_direction = Vec3f(1, 1, 1).normalize();
const Vec3f up(0, 1, 0);
const Vec3f eye(1, 1, 3);
const Vec3f center(0, 0, 0);

///////////////////////////////////////////////////////
/// shaders ///////////////////////////////////////////
///////////////////////////////////////////////////////

struct ShaderImpl : public Shader {
    // uniforms are constant values passed to the shader
    Mat4f uniform_mvp;
    Mat4f uniform_mvp_inv_T;

    // varyings are written by vertex shader, read by fragment shader
    Vec2f varying_uv[3];
    Vec3f varying_normal[3];
    Vec3f varying_position[3]; // vertices positions in NDC ([-1, 1])
    Vec4f varying_screen_coord[3]; // vertices positions in screen space

    Vec4f vertex(int iface, int nthvert) override {
        varying_uv[nthvert] = model->uv(iface, nthvert);
        varying_normal[nthvert] = model->normal(iface, nthvert);
        
        varying_screen_coord[nthvert] = uniform_mvp * Vec4f(model->position(iface, nthvert), 1);
        varying_position[nthvert] = varying_screen_coord[nthvert].homogenize().xyz();
        
        // NDC's [-1, 1] range transformed to screen coordinates by the MVP matrix
        return varying_screen_coord[nthvert];
    }

    bool fragment(Vec3f bary, TGAColor &color) override {
        // interpolate values for the current pixel

        Vec3f vertices_u_coords = Vec3f(varying_uv[0].x, varying_uv[1].x, varying_uv[2].x);
        Vec3f vertices_v_coords = Vec3f(varying_uv[0].y, varying_uv[1].y, varying_uv[2].y);
        Vec2f uv = Vec2f(
            Geometry::barycentric_interp(bary, vertices_u_coords),
            Geometry::barycentric_interp(bary, vertices_v_coords)
        );

        Vec3f vertices_x_normal_coords = Vec3f(varying_normal[0].x, varying_normal[1].x, varying_normal[2].x);
        Vec3f vertices_y_normal_coords = Vec3f(varying_normal[0].y, varying_normal[1].y, varying_normal[2].y);
        Vec3f vertices_z_normal_coords = Vec3f(varying_normal[0].z, varying_normal[1].z, varying_normal[2].z);
        Vec3f normal = Vec3f(
            Geometry::barycentric_interp(bary, vertices_x_normal_coords),
            Geometry::barycentric_interp(bary, vertices_y_normal_coords),
            Geometry::barycentric_interp(bary, vertices_z_normal_coords)
        ).normalize();

        Mat3f A;
        A.set_row(0, varying_position[1] - varying_position[0]);
        A.set_row(1, varying_position[2] - varying_position[0]);
        A.set_row(2, normal);

        Mat3f A_inv = A.inversed();

        Vec3f i = A_inv * Vec3f(varying_uv[1].x - varying_uv[0].x, varying_uv[2].x - varying_uv[0].x, 0);
        Vec3f j = A_inv * Vec3f(varying_uv[1].y - varying_uv[0].y, varying_uv[2].y - varying_uv[0].y, 0);

        Mat3f B;
        B.set_col(0, i.normalize());
        B.set_col(1, j.normalize());
        B.set_col(2, normal);

        Vec3f n = (B * model->normal_map_at(uv)).normalize();
        // Darboux basis = (i, j, n)

        float intensity = dot(n, light_direction);
        float diff = std::max(0.0f, intensity); // the light is behind when values are negative
        color = model->diffuse_map_at(uv) * diff;

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

    const Mat4f model_view = Transform::look_at(eye, center, up);
    const Mat4f projection = Transform::projection((eye - center).length());
    // const Mat4f projection = Transform::perspective(90, resolution.x / resolution.y, 0, 255);

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