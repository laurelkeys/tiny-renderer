#include "model.hh"
#include "geometry.hh"
#include "tgaimage.hh"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor green = TGAColor(  0, 255,   0, 255);
const TGAColor blue  = TGAColor(  0,   0, 255, 255);

const int WIDTH  = 200;
const int HEIGHT = 200;

void line(Vec2i p0, Vec2i p1, 
          TGAImage &image, const TGAColor &color) {
    bool transposed = false;
    if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {
        std::swap(p0.x, p0.y); // if the line is steep,
        std::swap(p1.x, p1.y); // we transpose the image
        transposed = true;
    }

    if (p0.x > p1.x) {
        std::swap(p0.x, p1.x);
        std::swap(p0.y, p1.y);
    }

    int dx = p1.x - p0.x;
    int dy = p1.y - p0.y;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    
    int y = p0.y;
    for (int x = p0.x; x <= p1.x; ++x) {
        if (transposed)
            image.set(y, x, color); // de−transpose
        else
            image.set(x, y, color);

        error2 += derror2;
        if (error2 > dx) {
            y += (p1.y > p0.y ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void triangle(Vec2i p0, Vec2i p1, Vec2i p2,
              TGAImage &image, const TGAColor &color) {
    if (p0.y == p1.y && p0.y == p2.y)
        return; // ignore degenerate triangles

    // sort the vertices by their y-coordinates
    if (p1.y < p0.y) std::swap(p0, p1);
    if (p2.y < p0.y) std::swap(p0, p2);
    if (p2.y < p1.y) std::swap(p1, p2);

    // A.y = B.y + C.y
    Vec2i A = p2 - p0;
    Vec2i B = p1 - p0;
    Vec2i C = p2 - p1;

    for (int y = 0; y <= A.y; ++y) {
        bool upper_side = (y > B.y) || (B.y == 0);

        // lerp segments
        int start = p0.x + A.x * (y / (float) A.y);
        int end = upper_side ? p1.x + C.x * ((y - B.y) / (float) C.y) 
                             : p0.x + B.x * (y / (float) B.y);
        if (start > end)
            std::swap(start, end);

        // obs.: we don't call line() to avoid its checks
        for (int x = start; x <= end; ++x)
            image.set(x, p0.y + y, color);
    }
}

int main(int argc, char **argv) {
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    
    Vec2i p0[3] = { Vec2i( 10,  70), Vec2i( 50, 160), Vec2i( 70,  80) };
    Vec2i p1[3] = { Vec2i(180,  50), Vec2i(150,   1), Vec2i( 70, 180) };
    Vec2i p2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };

    triangle(p0[0], p0[1], p0[2], image, white);
    triangle(p1[0], p1[1], p1[2], image, white);
    triangle(p2[0], p2[1], p2[2], image, white);

    image.flip_vertically(); // have the origin at the bottom-left corner
    image.write_tga_file("output.tga");

    return 0;
}
