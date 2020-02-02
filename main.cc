#include "colors.hh"
#include "model.hh"
#include "tgaimage.hh"

#include "Obj.hh"
#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec3f;

Model *model = nullptr;
Obj::Model *obj_model = nullptr;
const Vec2i resolution(800, 800);

int main(int argc, char **argv) {
    // model = new Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    using namespace std;
    
    cout << endl;
    cout << "Model" << endl;
    model = new Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");
    cout << "faces=" << model->nfaces() << endl;
    cout << "verts=" << model->nverts() << endl;
    
    cout << endl;
    cout << "Obj::Model" << endl;
    obj_model = new Obj::Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");
    cout << "faces=" << obj_model->n_of_faces() << endl;
    cout << "verts=" << obj_model->n_of_vertices() << endl;

    cout << endl;
    for (int i = 0; i < model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
        Obj::Face obj_face = obj_model->face(i);
        
        for (int f = 0; f < static_cast<int>(face.size()); ++f) {
            if (face[f] != obj_face[f].p)
                cout << "(i=" << i << ") face[" << f << "] != obj_face[" << f << "].p" << endl;

            Vec3f v = model->vert(face[f]);
            Vec3f obj_v = obj_model->position(obj_face[f].p);
            Obj::Vertex obj_vertex = obj_model->vertex(obj_face[f]);
            if (v != obj_v)
                cout << "(i=" << i << ") v != obj_v => " << v << "!=" << obj_v << endl;
            if (v != obj_vertex.pos)
                cout << "(i=" << i << ") v != obj_vertex.pos => " << v << "!=" << obj_vertex.pos << endl;
        }

    }

    delete model;
    delete obj_model;

    // int *z_buffer = new int[resolution.x * resolution.y];
    // std::fill_n(z_buffer, resolution.x * resolution.y, Math::MIN_INT);

    // TGAImage image(resolution.x, resolution.y, TGAImage::RGB);
    // Vec3f light_direction(0, 0, -1);

    // for (int i = 0; i < model->nfaces(); i++) {
    //     std::vector<int> face = model->face(i);

    //     // store the three vertices of each triangle
    //     Vec3f world_coords[3];
    //     Vec3i screen_coords[3];
    //     for (int j = 0; j < 3; j++) {
    //         Vec3f v = model->vert(face[j]);
    //         world_coords[j] = v; // [-1, 1]
    //         screen_coords[j] = Vec3i(
    //             (v.x + 1.) * resolution.x / 2., // map [-1, 1] to [0, width]
    //             (v.y + 1.) * resolution.y / 2., // map [-1, 1] to [0, height]
    //             (v.z + 1.) * 255 / 2.           // map [-1, 1] to [0, 255]
    //         );
    //     }

    //     Vec3f n = cross(
    //         world_coords[2] - world_coords[0], // use the cross product of two segments
    //         world_coords[1] - world_coords[0]  // of the triangle to compute a face normal
    //     ).normalize();
    
    //     float intensity = n.dot(light_direction);
    //     // intensity < 0 means the light is coming from behind the polygon,
    //     // so we ignore it (obs.: this is called back-face culling)
    //     if (intensity > 0) {
    //         Draw::triangle(
    //             screen_coords[0], screen_coords[1], screen_coords[2], 
    //             z_buffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255)
    //         );
    //     }
    // }

    // image.flip_vertically(); // have the origin at the bottom left corner of the image
    // image.write_tga_file("output.tga");

    // delete model;
    // delete[] z_buffer;
    // return 0;
}