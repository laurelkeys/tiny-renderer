#include "colors.hh"
#include "tgaimage.hh"

#include "Obj.hh"
#include "Draw.hh"
#include "Math.hh"
#include "Types.hh"
#include "Shader.hh"
#include "Geometry.hh"
#include "Transform.hh"
#include "Primitives.hh"

using Types::Vec2i;
using Types::Vec3i;

using Types::Vec2f;
using Types::Vec3f;

using Types::Vec4f; // homogeneous coordinates (points with w=1, vectors with w=0)
using Types::Mat4f;

///////////////////////////////////////////////////////
/// scene /////////////////////////////////////////////
///////////////////////////////////////////////////////

Obj::Model *model = nullptr;
const Vec2i resolution(800, 800);

const Vec3f light_direction = Vec3f(2, 2, 1).normalize();
const Vec3f up(0, 1, 0);
const Vec3f eye(1, -1, 3); // (0, 0, c)
const Vec3f center(0, 0, 0);

const Mat4f model_view = Transform::look_at(eye, center, up);
const Mat4f projection = Transform::projection((eye - center).length());
const Mat4f viewport = Transform::viewport(resolution.x, resolution.y);

///////////////////////////////////////////////////////
/// shaders ///////////////////////////////////////////
///////////////////////////////////////////////////////

struct GouraudShader : public Shader {
    Vec3f varying_intensity; // written by vertex shader, read by fragment shader

    virtual Vec4f vertex(int iface, int nthvert) override {
        // get diffuse lighting intensity
        // obs.: use max(0, intensity), as negative values mean the light is behind
        float intensity = dot(model->normal(iface, nthvert), light_direction);
        varying_intensity[nthvert] = intensity > 0 ? intensity : 0;

        // read the vertex position from the .obj model file
        // obs.: they're in normalized device coordinates (NDC), i.e. [-1, 1] range
        Vec4f vertex_coords = Vec4f(model->position(iface, nthvert), 1);

        // transform it to screen coordinates
        return viewport * projection * model_view * vertex_coords;
    }

    virtual bool fragment(Vec3f bary, TGAColor &color) override {
        // interpolate intensity for the current pixel
        float intensity = Geometry::barycentric_interp(bary, varying_intensity);
        color = TGAColor(255, 255, 255) * intensity;

        // return false to signal that we won't discard this pixel
        return false;
    }
};

///////////////////////////////////////////////////////
/// main //////////////////////////////////////////////
///////////////////////////////////////////////////////

int main(int argc, char **argv) {
    model = new Obj::Model(argc >= 2 ? argv[1] : "obj/african_head/african_head.obj");

    TGAImage image(resolution.x, resolution.y, TGAImage::RGB);
    int *z_buffer = new int[resolution.x * resolution.y];
    for (int i = 0; i < resolution.x * resolution.y; ++i) {
        z_buffer[i] = Math::MIN_INT;
    }

    GouraudShader shader;
    for (int i = 0; i < model->n_of_faces(); ++i) {
        Vec3i screen_coords[3];
        for (int j = 0; j < 3; ++j) {
            // FIXME make screen_coords Vec4f instead
            screen_coords[j] = Vec3i((
                shader.vertex(i, j)
            ).homogenize().xyz());
        }

        Draw::triangle(
            screen_coords, shader,
            z_buffer, image, model
        );
    }

    image.flip_vertically(); // have the origin at the bottom left corner of the image
    image.write_tga_file("output.tga");

    delete model;
    delete[] z_buffer;
    return 0;
}