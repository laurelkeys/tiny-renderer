#include "colors.hh"
#include "model.hh"
#include "tgaimage.hh"

#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"

using Types::Vec2i;
using Types::Vec3f;

Model *model = NULL;
const Vec2i resolution(800, 800);

int main(int argc, char **argv) {
    model = new Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        
        Vec2i screen_coords[3]; // three vertices
        for (int j = 0; j < 3; j++) {
            Vec3f world_coords = model->vert(face[j]);
            screen_coords[j] = Vec2i(
                (world_coords.x + 1.) * resolution.x / 2., // map [-1, 1] to [0, width]
                (world_coords.y + 1.) * resolution.y / 2.  // map [-1, 1] to [0, height]
            );
        }
        
        Draw::triangle(
            screen_coords[0], screen_coords[1], screen_coords[2], 
            image, TGAColor(rand() % 255, rand() % 255, rand() % 255)
        );
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    delete model;
    return 0;
}