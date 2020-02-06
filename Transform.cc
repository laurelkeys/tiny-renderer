#include "Transform.hh"

#include "Math.hh"

using Types::Vec3f;

using Types::Mat4f;

namespace Transform {

    Mat4f translate(const Vec3f &v) {
        return Mat4f(
            1, 0, 0, v.x,
            0, 1, 0, v.y,
            0, 0, 1, v.z,
            0, 0, 0,  1
        );
    }

    Mat4f scale(const Vec3f &v) {
        assert(v != Vec3f(0));
        return Mat4f(
            v.x,  0,   0,  0,
             0,  v.y,  0,  0,
             0,   0,  v.z, 0,
             0,   0,   0,  1
        );
    }

    Mat4f scale(float a) {
        return scale(Vec3f(a, a, a));
    }

    Mat4f rotate(float angle_deg, const Vec3f &axis) {
        // http://docs.gl/gl2/glRotate

        float angle_rad = Math::deg2rad(angle_deg);
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        Vec3f n = axis.normalized();

        float x = n.x, y = n.y, z = n.z;
        return Mat4f(
            x*x*(1-c)+ c , x*y*(1-c)-z*s, x*z*(1-c)+y*s, 0,
            y*x*(1-c)+z*s, y*y*(1-c)+ c , y*z*(1-c)-x*s, 0,
            x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+ c , 0,
                  0      ,       0      ,       0      , 1
        );
    }

    Mat4f rotate_x(float angle_deg) {
        float angle_rad = Math::deg2rad(angle_deg);
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Mat4f(
            1, 0,  0, 0,
            0, c, -s, 0,
            0, s,  c, 0,
            0, 0,  0, 1
        );
    }

    Mat4f rotate_y(float angle_deg) {
        float angle_rad = Math::deg2rad(angle_deg);
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Mat4f(
             c, 0, s, 0,
             0, 1, 0, 0,
            -s, 0, c, 0,
             0, 0, 0, 1
        );
    }

    Mat4f rotate_z(float angle_deg) {
        float angle_rad = Math::deg2rad(angle_deg);
        float c = std::cos(angle_rad);
        float s = std::sin(angle_rad);
        return Mat4f(
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        );
    }

    Mat4f viewport(int leftmost_x, int bottommost_y, int width, int height) {
        float w = 0.5f * width, h = 0.5f * height, d = 0.5f;
        // map normalized device coordinates to screen coordinates:
        return Mat4f(
            w, 0, 0, w + leftmost_x,   // [-1, 1] -> [lm_x, width]
            0, h, 0, h + bottommost_y, // [-1, 1] -> [bm_y, height]
            0, 0, d, d,                // [-1, 1] -> [0, 1]
            0, 0, 0, 1
        );
    }

    Mat4f viewport(int width, int height) {
        return viewport(0, 0, width, height);
    }

    Mat4f look_at(const Vec3f &eye, const Vec3f &target, const Vec3f &up) {
        Vec3f z = (eye - target).normalize(); //  y |
        Vec3f x = cross(up, z).normalize();   //    o —— x
        Vec3f y = cross(z, x);                // z /
        return Mat4f(
            x.x, x.y, x.z, -dot(x, eye),
            y.x, y.y, y.z, -dot(y, eye),
            z.x, z.y, z.z, -dot(z, eye),
             0 ,  0 ,  0 ,       1
        );
    }
}