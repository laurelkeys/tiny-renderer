#include "colors.hh"
#include "model.hh"
#include "tgaimage.hh"

#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"

using Types::Vec2i;
using Types::Vec3f;

Model *model = nullptr;
const Vec2i resolution(200, 200);

int main(int argc, char **argv) {
    // model = new Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);

    Draw::triangle(Vec2i(10, 10), Vec2i(100, 30), Vec2i(190, 160), image, red);

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    // delete model;
    return 0;
}