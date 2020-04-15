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
const Vec3f center(0, 0, 0); // target
const Vec3f light_direction = Vec3f(1, 1, 1).normalize();

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: ./" << argv[0]
                  << " path/to/model.obj"
                  << std::endl;
        return 1;
    }

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);
    float *z_buffer = new float[resolution.x * resolution.y];
    for (int i = 0; i < resolution.x * resolution.y; ++i) {
        z_buffer[i] = Math::MIN_FLOAT;
    }

    const Mat4f model_view = Transform::look_at(eye, center, up);
    const Mat4f projection = Transform::projection((eye - center).length());
    const Mat4f mvp = projection * model_view;

    Shaders::Phong shader;
    shader.uniform_viewport = viewport;
    shader.uniform_mvp = mvp;
    shader.uniform_mvp_inv_T = mvp.inversed().transposed();
    shader.uniform_light_direction = (mvp * Vec4f(light_direction, 0)).xyz().normalize();

    shader.uniform_ka = 5;
    shader.uniform_kd = 1;
    shader.uniform_ks = 0.6;

    for (int m = 1; m < argc; ++m) {
        model = new Obj::Model(argv[m]);
        shader.uniform_model = model;
        for (int i = 0; i < model->n_of_faces(); ++i) {
            Vec3f screen_coords[3];
            for (int j = 0; j < 3; ++j)
                screen_coords[j] = shader.vertex(i, j);
            Draw::triangle(screen_coords, shader, image, z_buffer);
        }
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    delete model;
    delete[] z_buffer;
    return 0;
}

//
// 1. argparse:
//    - resolution
//    - up
//    - eye
//    - center
//    - light_direction
//    - output filename
// 2. save output as .png or .jpg
// 3. better handle textures (missing ones / other names)
// 4. parse .obj files with faces that are not triangular
// 5. allow shaders to be written as separate files (requires parsing)
// 6. treat barycentric coordinates as "weights" to simplify interpolation code
//