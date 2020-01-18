#include <limits>
#include <memory>
#include <algorithm>

#include "model.hh"
#include "geometry.hh"
#include "tgaimage.hh"

const float MIN_INT = std::numeric_limits<int>::lowest();
const float MAX_FLOAT = std::numeric_limits<float>::max();
const float MIN_FLOAT = std::numeric_limits<float>::lowest(); // -MAX_FLOAT
const float EPS_FLOAT = std::numeric_limits<float>::epsilon();

const int WIDTH  = 800;
const int HEIGHT = 800;
const int DEPTH  = 255;

Model *model = nullptr;

Vec3f barycentric_coords(const Vec2i &A, const Vec2i &B, const Vec2i &C, const Vec2i &P) {
    Vec2i AB = B - A;
    Vec2i AC = C - A;
    Vec2i PA = A - P;

    // given a triangle ABC and a point P, defined by:
    //   P = (1-u-v) * A + u * B + v * C = A + u * AB + v * AC,
    // P is inside the triangle iff:
    //   (0 <= u, v <= 1) and (u + v <= 1)
    Vec3i coords = cross(Vec3i(AC.x, AB.x, PA.x),
                         Vec3i(AC.y, AB.y, PA.y));

    if (coords.z == 0) {
        // the triangle is degenerate, so we return a negative coordinate
        // for it to be skipped (as it means P lies outside triangle ABC)
        return Vec3f(-1, 1, 1);
    }

    return Vec3f(1.0f - (coords.x + coords.y) / static_cast<float>(coords.z), // 1 - u - v
                 coords.y / static_cast<float>(coords.z),  // u = (PA.x * AC.y - AC.x * PA.y) / (AC.x * AB.y - AB.x * AC.y)
                 coords.x / static_cast<float>(coords.z)); // v = (AB.x * PA.y - AB.y * PA.x) / (AC.x * AB.y - AB.x * AC.y)
}

struct Vertex {
    Vec3i pos;
    Vec2i uv;
};

void triangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, 
              float intensity[3], int z_buffer[WIDTH * HEIGHT], TGAImage &image) {
    const Vec2i bbox_min(std::max(0, std::min(std::min(v0.pos.x, v1.pos.x), v2.pos.x)),
                         std::max(0, std::min(std::min(v0.pos.y, v1.pos.y), v2.pos.y)));
    const Vec2i bbox_max(std::min(image.get_width() - 1, std::max(std::max(v0.pos.x, v1.pos.x), v2.pos.x)),
                         std::min(image.get_height() - 1, std::max(std::max(v0.pos.y, v1.pos.y), v2.pos.y)));
    
    const Vec3f vertex_heights(v0.pos.z, v1.pos.z, v2.pos.z);
    
    Vec2i P;
    for (P.x = bbox_min.x; P.x <= bbox_max.x; ++P.x) {
        for (P.y = bbox_min.y; P.y <= bbox_max.y; ++P.y) {
            Vec3f bc_screen = barycentric_coords(v0.pos.xy(), v1.pos.xy(), v2.pos.xy(), P);

            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue; // point lies outside the triangle

            // NOTE using P.z would implicitly cast from float to int since P is a Vec3i
            float Pz = bc_screen * vertex_heights; // P's height (z) is a "weighted sum"
            if (z_buffer[int(P.x + P.y * WIDTH)] < Pz) {
                z_buffer[int(P.x + P.y * WIDTH)] = Pz;
                Vec3f bc = barycentric_coords(v0.uv, v1.uv, v2.uv, P);
                // TGAColor color = model->diffuse(Vec2i(bc.x, bc.y));
                TGAColor color(255 * intensity[0], 
                               255 * intensity[1], 
                               255 * intensity[2]);
                image.set(P.x, P.y, color);
            }
        }
    }
}

int main(int argc, char **argv) {
    model = new Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    int *z_buffer = new int[WIDTH * HEIGHT];
    std::fill_n(z_buffer, WIDTH * HEIGHT, MIN_INT);

    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    Vec3f light_direction(0, 0, -1);

    for (int i = 0; i < model->nfaces(); ++i) {
        std::vector<int> face = model->face(i);

        float intensity[3];
        Vec3f world_coords[3];
        Vec3i screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            intensity[j] = model->normal(i, j) * light_direction;
            world_coords[j] = model->vert(face[j]);
            screen_coords[j] = Vec3i((world_coords[j].x + 1) * WIDTH / 2, 
                                     (world_coords[j].y + 1) * HEIGHT / 2, 
                                     (world_coords[j].z + 1) * DEPTH / 2);
        }

        // intensity < 0 means the light is coming from behind the polygon,
        // so we ignore it (obs.: this is called back-face culling)
        // if (intensity > 0) {
        Vec2i uv[3] = { model->uv(i, 0), model->uv(i, 1), model->uv(i, 2) };
        triangle({ screen_coords[0], uv[0] },
                 { screen_coords[1], uv[1] },
                 { screen_coords[2], uv[2] },
                 intensity, z_buffer, image);
        // }
    }

    image.flip_vertically(); // have the origin at the bottom-left corner
    image.write_tga_file("output.tga");

    { // dump z-buffer (debugging purposes only)
        TGAImage zbimage(WIDTH, HEIGHT, TGAImage::GRAYSCALE);
        for (int i = 0; i < WIDTH; i++)
            for (int j = 0; j < HEIGHT; j++)
                zbimage.set(i, j, TGAColor(z_buffer[i + j * WIDTH]));
        zbimage.flip_vertically();
        zbimage.write_tga_file("zbuffer.tga");
    }

    delete model;
    delete[] z_buffer;
}
