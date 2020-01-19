#include <limits>
#include <memory>
#include <algorithm>

#include "model.hh"
// #include "geometry.hh"
#include "tgaimage.hh"

#include "math/Vec.hh"
#include "math/Mat4f.hh"

const float MIN_INT = std::numeric_limits<int>::lowest();
const float MAX_FLOAT = std::numeric_limits<float>::max();
const float MIN_FLOAT = std::numeric_limits<float>::lowest(); // -MAX_FLOAT
const float EPS_FLOAT = std::numeric_limits<float>::epsilon();

const TGAColor black   = TGAColor( 0 ,  0 ,  0 );
const TGAColor white   = TGAColor(255, 255, 255);
const TGAColor red     = TGAColor(255,  0 ,  0 ); // R
const TGAColor green   = TGAColor( 0 , 255,  0 ); // G
const TGAColor blue    = TGAColor( 0 ,  0 , 255); // B
const TGAColor yellow  = TGAColor(255, 255,  0 ); // Y
const TGAColor magenta = TGAColor(255,  0 , 255); // M
const TGAColor cyan    = TGAColor( 0 , 255, 255); // C

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

void triangle(const Vec3f &p0, const Vec3f &p1, const Vec3f &p2, 
              const Vec2f &uv0, const Vec2f &uv1, const Vec2f &uv2, 
              float intensity, int z_buffer[WIDTH * HEIGHT], TGAImage &image) {
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
            float Pz = dot(bc_screen, vertex_heights); // P's height (z) is a "weighted sum"
            if (z_buffer[int(P.x + P.y * WIDTH)] < Pz) {
                z_buffer[int(P.x + P.y * WIDTH)] = Pz;
                // TGAColor color(intensity * 255);
                Vec3f bc = barycentric_coords(uv0, uv1, uv2, P);
                TGAColor color = model->diffuse(Vec2i(bc.x, bc.y));
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

    for (int i = 0; i < model->n_of_faces(); ++i) {
        std::vector<int> face = model->face(i); // list of vertex indices

        Vec3f world_coords[3];
        Vec3i screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            // intensity[j] = dot(model->normal(i, j), light_direction);
            world_coords[j] = model->vertex(face[j]);
            screen_coords[j] = Vec3i((world_coords[j].x + 1) * WIDTH / 2, 
                                     (world_coords[j].y + 1) * HEIGHT / 2, 
                                     (world_coords[j].z + 1) * DEPTH / 2);
        }

        Vec3f face_normal = cross(world_coords[2] - world_coords[0],
                                  world_coords[1] - world_coords[0]).normalize();

        float intensity = dot(light_direction, face_normal);
        // intensity < 0 means the light is coming from behind the polygon,
        // so we ignore it (obs.: this is called back-face culling)
        if (intensity > 0) {
            triangle(Vertex(screen_coords[0], model->uv(i, 0)),
                     Vertex(screen_coords[1], model->uv(i, 1)),
                     Vertex(screen_coords[2], model->uv(i, 2)),
                     intensity, z_buffer, image);
        }
    }

    image.flip_vertically(); // have the origin at the bottom-left corner
    image.write_tga_file("output.tga");

    delete model;
    delete[] z_buffer;
}

// Matrix:             Model          View           Projection
// Coordinates: Model -------> World ------> Camera ------------> Homogeneous
// (http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/)