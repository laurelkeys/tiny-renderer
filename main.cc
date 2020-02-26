#include "colors.hh"
#include "tgaimage.hh"

#include "Obj.hh"
#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"
#include "Shaders.hh"
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


Obj::Model *model = nullptr;
const Vec2i resolution(800, 800);
const Mat4f viewport = Transform::viewport(resolution.x, resolution.y, 1);

const Vec3f up(0, 1, 0);
const Vec3f eye(0, 0, 3); // (0, 0, focal_dist)
const Vec3f center(0, 0, 0);
const Vec3f light_direction = Vec3f(1, 1, 1).normalize();


int main(int argc, char **argv) {
    model = new Obj::Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);
    float *z_buffer = new float[resolution.x * resolution.y];
    for (int i = 0; i < resolution.x * resolution.y; ++i) {
        z_buffer[i] = Math::MIN_FLOAT; // FIXME should also work with -Math::EPS_FLOAT
    }

    const Mat4f model_view = Transform::look_at(eye, center, up);
    const Mat4f projection = Transform::projection((eye - center).length());

    Shaders::Texture shader;
    shader.uniform_model = model;
    shader.uniform_light_direction = light_direction;

    Mat4f mvp = projection * model_view;
    shader.uniform_mvp       = mvp;
    shader.uniform_mvp_inv_T = mvp.inversed().transposed();
    shader.uniform_viewport  = viewport;

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