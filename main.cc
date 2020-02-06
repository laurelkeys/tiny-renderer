#include "colors.hh"
#include "tgaimage.hh"

#include "Obj.hh"
#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"
#include "Primitives.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

Obj::Model *model = nullptr;
const Vec2i resolution(800, 800);
int main(int argc, char **argv) {
    model = new Obj::Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    int *z_buffer = new int[resolution.x * resolution.y];
    std::fill_n(z_buffer, resolution.x * resolution.y, Math::MIN_INT);

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);
    Vec3f light_direction(0, 0, -1);

    for (int i = 0; i < model->n_of_faces(); i++) {
        Primitives::Face face = model->face(i);

        // store the three vertices' positions of each triangle
        Vec3f world_coords[3];
        Vec3i screen_coords[3];
        // store the three vertices' uv texture coordinates of each triangle
        Vec2f uv_coords[3];
        for (int j = 0; j < 3; j++) {
            Vec3f v = face[j].pos;
            world_coords[j] = v; // [-1, 1] (position of the j-th face vertex)
            screen_coords[j] = Vec3i(
                (v.x + 1.) * resolution.x / 2., // map [-1, 1] to [0, width]
                (v.y + 1.) * resolution.y / 2., // map [-1, 1] to [0, height]
                (v.z + 1.) * 255 / 2.           // map [-1, 1] to [0, 255]
            );
            uv_coords[j] = face[j].uv;
        }

        Vec3f n = cross(
            world_coords[2] - world_coords[0], // use the cross product of two segments
            world_coords[1] - world_coords[0]  // of the triangle to compute a face normal
        ).normalize();

        float intensity = n.dot(light_direction);
        // intensity < 0 means the light is coming from behind the polygon,
        // so we ignore it (obs.: this is called back-face culling)
        if (intensity > 0) {
            Draw::triangle(
                screen_coords, uv_coords,
                z_buffer, image, model
            );
        }
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    delete model;
    delete[] z_buffer;
    return 0;
}