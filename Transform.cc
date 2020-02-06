#include "Transform.hh"

#include "Math.hh"

using Types::Vec3f;

using Types::Mat4f;

namespace Transform {

    static Mat4f translate(const Vec3f &v) {
        return Mat4f(
            1, 0, 0, v.x,
            0, 1, 0, v.y,
            0, 0, 1, v.z,
            0, 0, 0,  1
        );
    }

    static Mat4f scale(const Vec3f &v) {
        assert(v != Vec3f(0));
        return Mat4f(
            v.x,  0,   0,  0,
             0,  v.y,  0,  0,
             0,   0,  v.z, 0,
             0,   0,   0,  1
        );
    }

    static Mat4f rotate(float angle_deg, const Vec3f &axis) {
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

    static Mat4f rotate_x(float angle_deg) {
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

    static Mat4f rotate_y(float angle_deg) {
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

    static Mat4f rotate_z(float angle_deg) {
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
}