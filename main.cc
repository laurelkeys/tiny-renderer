#include "TGAImage.hh"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

int main(int argc, char **argv) {
	const int width = 100, height = 100;
    
	TGAImage image(width, height, TGAImage::RGB);
    
	image.set(52, 41, red);
    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");
    return 0;
}
