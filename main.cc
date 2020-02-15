#include "colors.hh"
#include "tgaimage.hh"

#include "Obj.hh"
#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"
#include "Transform.hh"
#include "Primitives.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

using Types::Vec4f; // homogeneous coordinates (points with w=1, vectors with w=0)
using Types::Mat4f;

Obj::Model *model = nullptr;
const Vec2i resolution(800, 800);

int main(int argc, char **argv) {
    model = new Obj::Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    int *z_buffer = new int[resolution.x * resolution.y];
    for (int i = 0; i < resolution.x * resolution.y; ++i) {
        z_buffer[i] = Math::MIN_INT;
    }

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);

    Vec3f light_direction(0, 0, -1);
    Vec3f eye(0, 0, 3); // (0, 0, c)
    Vec3f center(0, 0, 0);

    Mat4f model_view = Transform::look_at(eye, center, Vec3f(0, 1, 0));
    Mat4f projection = Transform::projection(eye, center);
    Mat4f viewport = Transform::viewport(
        resolution.x * 0.125, resolution.y * 0.125, // 1 / 8
        resolution.x * 0.750, resolution.y * 0.750  // 3 / 4
    );

    std::cerr << "\nmodel_view:" << std::endl;
    std::cerr << model_view << std::endl;
    std::cerr << "projection:" << std::endl;
    std::cerr << projection << std::endl;
    std::cerr << "viewport:" << std::endl;
    std::cerr << viewport << std::endl;
    std::cerr << "projection * model_view:" << std::endl;
    std::cerr << projection * model_view << std::endl;
    std::cerr << "viewport * projection * model_view:" << std::endl;
    std::cerr << viewport * projection * model_view << std::endl;

    for (int i = 0; i < model->n_of_faces(); ++i) {
        Primitives::Face face = model->face(i);

        Vec2f uv_coords[3];
        Vec3f world_coords[3];
        Vec3i screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            Vec3f v = face[j].pos; // position of the j-th face vertex
            world_coords[j] = v; // normalized device coordinates (i.e. [-1, 1] range)
            // apply transformations and map NDC to [0, width] x [0, height] x [0, 255]
            screen_coords[j] = Vec3i((
               viewport * projection * Vec4f(v, 1)
            ).homogenize().xyz());

            uv_coords[j] = face[j].uv; // uv texture coordinates (-1..1)
        }

        Vec3f n = cross(
            world_coords[2] - world_coords[0], // use the cross product of two segments
            world_coords[1] - world_coords[0]  // of the triangle to compute a face normal
        ).normalize();

        float intensity = dot(n, light_direction);
        // intensity < 0 means the light is coming from behind the polygon,
        // so we ignore it (obs.: this is called back-face culling)
        if (intensity > 0) {
            Draw::triangle(
                screen_coords, uv_coords,
                intensity, z_buffer, image, model
            );
        }
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    delete model;
    delete[] z_buffer;
    return 0;
}