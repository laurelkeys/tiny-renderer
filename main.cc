#include "model.hh"
#include "geometry.hh"
#include "tgaimage.hh"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor green = TGAColor(  0, 255,   0, 255);
const TGAColor blue  = TGAColor(  0,   0, 255, 255);

Model *model = nullptr;
const int WIDTH  = 800;
const int HEIGHT = 800;
const int HALF_WIDTH  = 400;
const int HALF_HEIGHT = 400;

void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color) {
    bool transposed = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0); // if the line is steep,
        std::swap(x1, y1); // we transpose the image
        transposed = true;
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    
    int y = y0;
    for (int x = x0; x <= x1; ++x) {
        if (transposed)
            image.set(y, x, color); // de−transpose
        else
            image.set(x, y, color);

        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

int main(int argc, char **argv) {
    if (argc >= 2) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/head.obj");
    }

    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    for (int i = 0; i < model->nfaces(); ++i) {
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; ++j) {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j + 1) % 3]);
            
            int x0 = (v0.x + 1) * HALF_WIDTH;
            int y0 = (v0.y + 1) * HALF_HEIGHT;
            int x1 = (v1.x + 1) * HALF_WIDTH;
            int y1 = (v1.y + 1) * HALF_HEIGHT;
            
            line(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically(); // have the origin at the bottom-left corner
    image.write_tga_file("output.tga");

    delete model;
    return 0;
}
