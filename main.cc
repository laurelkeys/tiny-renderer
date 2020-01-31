#include "model.hh"
#include "colors.hh"
#include "tgaimage.hh"

#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"

using Types::Vec2i;

const Vec2i resolution(100, 100);

int main(int argc, char **argv) {
    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);

    Draw::line({ 13, 20 }, { 80, 40 }, image, white);
    Draw::line({ 20, 13 }, { 40, 80 }, image, red);
    // Draw::line({ 80, 40 }, { 13, 20 }, image, red);

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");
    return 0;
}