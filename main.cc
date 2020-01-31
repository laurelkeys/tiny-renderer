#include "model.hh"
#include "colors.hh"
#include "tgaimage.hh"

#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"

using Types::Vec2i;
using Types::Vec3f;

Model *model = nullptr;
const Vec2i resolution(100, 100);

int main(int argc, char **argv) {
    model = new Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            int x0 = (v0.x + 1.) * resolution.x / 2.;
            int y0 = (v0.y + 1.) * resolution.y / 2.;
            int x1 = (v1.x + 1.) * resolution.x / 2.;
            int y1 = (v1.y + 1.) * resolution.y / 2.;
            Draw::line(Vec2i(x0, y0), Vec2i(x1, y1), image, white);
        }
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}
