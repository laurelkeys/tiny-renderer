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

    int *z_buffer = new int[resolution.x * resolution.y];
    std::fill_n(z_buffer, resolution.x * resolution.y, Math::MIN_INT);

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);
    Vec3f light_direction(0, 0, -1);

    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);

        // store the three vertices of each triangle
        Vec3f world_coords[3];
        Vec2i screen_coords[3];
        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j]);
            world_coords[j] = v; // [-1, 1]
            screen_coords[j] = Vec2i(
                (v.x + 1.) * resolution.x / 2., // map [-1, 1] to [0, width]
                (v.y + 1.) * resolution.y / 2.  // map [-1, 1] to [0, height]
            );
        }

        Vec3f n = cross(
            world_coords[2] - world_coords[0], // use the cross product of two segments
            world_coords[1] - world_coords[0]  // of the triangle to compute a face normal
        ).normalize();
    
        float intensity = n.dot(light_direction);
        // intensity < 0 means the light is coming from behind the polygon,
        // so we ignore it (obs.: this is called back-face culling)
        if (intensity > 0) {
            Draw::triangle(
                screen_coords[0], screen_coords[1], screen_coords[2], 
                z_buffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255)
            );
        }
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    delete model;
    delete[] z_buffer;
    return 0;
}