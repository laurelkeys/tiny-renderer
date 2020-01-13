#include <limits>
#include <memory>
#include <algorithm>

#include "model.hh"
#include "geometry.hh"
#include "tgaimage.hh"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255,   0,   0, 255);
const TGAColor green = TGAColor(  0, 255,   0, 255);
const TGAColor blue  = TGAColor(  0,   0, 255, 255);

const int WIDTH  = 800;
const int HEIGHT = 800;
const int DEPTH  = 255;

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

Vec3f barycentric_coords(const Vec2i &A, const Vec2i &B, const Vec2i &C, const Vec2i &P) {
    Vec2i AB = B - A;
    Vec2i AC = C - A;
    Vec2i PA = A - P;

    // given a triangle ABC and a point P, defined by:
    //   P = (1-u-v) * A + u * B + v * C = A + u * AB + v * AC,
    // P is inside the triangle iff:
    //   (0 <= u, v <= 1) and (u + v <= 1)
    Vec3f coords = cross(Vec3f(AC.x, AB.x, PA.x),
                         Vec3f(AC.y, AB.y, PA.y));

    return Vec3f(1.0f - (coords.x + coords.y) / coords.z, // 1 - u - v
                 coords.y / coords.z,  // u = (PA.x * AC.y - AC.x * PA.y) / (AC.x * AB.y - AB.x * AC.y)
                 coords.x / coords.z); // v = (AB.x * PA.y - AB.y * PA.x) / (AC.x * AB.y - AB.x * AC.y)   
}

void triangle(const Vec3i &p0, const Vec3i &p1, const Vec3i &p2, 
              float z_buffer[WIDTH * HEIGHT], 
              TGAImage &image, const TGAColor &color) {
    const Vec2i bbox_min(std::max(0, std::min(std::min(p0.x, p1.x), p2.x)),
                         std::max(0, std::min(std::min(p0.y, p1.y), p2.y)));
    const Vec2i bbox_max(std::min(image.get_width() - 1, std::max(std::max(p0.x, p1.x), p2.x)),
                         std::min(image.get_height() - 1, std::max(std::max(p0.y, p1.y), p2.y)));
    
    const Vec3f vertex_heights(p0.z, p1.z, p2.z);
    
    Vec2i P;
    for (P.x = bbox_min.x; P.x <= bbox_max.x; ++P.x) {
        for (P.y = bbox_min.y; P.y <= bbox_max.y; ++P.y) {
            Vec3f bc_screen = barycentric_coords(p0.xy(), p1.xy(), p2.xy(), P);
            
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue; // point lies outside the triangle

            // NOTE using P.z would implicitly cast from float to int since P is a Vec3i
            float Pz = bc_screen * vertex_heights; // P's height (z) is a "weighted sum"
            if (z_buffer[int(P.x + P.y * WIDTH)] < Pz) {
                z_buffer[int(P.x + P.y * WIDTH)] = Pz;
                image.set(P.x, P.y, color);
            }
        }
    }
}

int main(int argc, char **argv) {
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    auto model = std::make_unique<Model>(argc >= 2 ? argv[1]
                                                   : "obj/african_head/african_head.obj");

    Vec3f light_direction(0.0, 0.0, -1.0);

    float *z_buffer = new float[WIDTH * HEIGHT];
    std::fill_n(z_buffer, WIDTH * HEIGHT, std::numeric_limits<float>::lowest());

    for (int i = 0; i < model->nfaces(); ++i) {
        std::vector<int> face = model->face(i);

        Vec3f world_coords[3];
        Vec3i screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            world_coords[j] = model->vert(face[j]);
            screen_coords[j] = Vec3i((world_coords[j].x + 1) * WIDTH / 2, 
                                     (world_coords[j].y + 1) * HEIGHT / 2, 
                                     (world_coords[j].z + 1) * DEPTH / 2);
        }

        Vec3f normal = cross(world_coords[2] - world_coords[0],
                             world_coords[1] - world_coords[0]).normalize();
        float intensity = light_direction * normal;
        // intensity < 0 means the light is coming from behind the polygon,
        // so we ignore it (obs.: this is called back-face culling)
        if (intensity > 0) {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], z_buffer, 
                     image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }

    image.flip_vertically(); // have the origin at the bottom-left corner
    image.write_tga_file("output.tga");
}
