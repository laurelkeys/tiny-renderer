#include "colors.hh"
#include "tgaimage.hh"
#include "model.h"
// #include "Obj.hh"
#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"
#include "Primitives.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

Model *model = nullptr;
const Vec2i resolution(800, 800);
int main(int argc, char **argv) {
    model = new Model(argc >= 2 ? argv[1] : "obj/cube.obj");

    // int *z_buffer = new int[resolution.x * resolution.y];
    // std::fill_n(z_buffer, resolution.x * resolution.y, Math::MIN_INT);

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);
    Vec3f light_direction(0, 0, -1);

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i); // vertex indices
        int nverts_in_face = static_cast<int>(face.size());
        for (int j = 0; j < nverts_in_face; j++) {
            // woorld coordinates:
            Vec3f wp0 = model->vert(face[j]);
            Vec3f wp1 = model->vert(face[(j + 1) % nverts_in_face]);
            // screen coordinates:
            { // draw the original model
                Vec3f wp0 = model->vert(face[j]);
                Vec3f wp1 = model->vert(face[(j + 1) % nverts_in_face]);
            }
            { // draw the deformed model

            }
            // screen_coords[j] = Vec3i(
            //     (v.x + 1.) * resolution.x / 2., // map [-1, 1] to [0, width]
            //     (v.y + 1.) * resolution.y / 2., // map [-1, 1] to [0, height]
            //     (v.z + 1.) * 255 / 2.           // map [-1, 1] to [0, 255]
            // );
        }
        break;
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    delete model;
    // delete[] z_buffer;
    return 0;
}