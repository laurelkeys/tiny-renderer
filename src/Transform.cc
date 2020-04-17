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
        // ref.: http://docs.gl/gl2/glRotate

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

    Mat4f viewport(int min_x, int min_y, int width, int height, int near, int far) {
        float w = 0.5f * width;
        float h = 0.5f * height;
        float d = 0.5f * (far - near); // 0.5f * depth
        return Mat4f(
            w, 0, 0, w + min_x, // [-1, 1] -> [min_x, min_x + width]
            0, h, 0, h + min_y, // [-1, 1] -> [min_y, min_y + height]
            0, 0, d, d + near,  // [-1, 1] -> [near, far] (usually the z-buffer "resolution")
            0, 0, 0, 1
        );
    }

    Mat4f viewport(int width, int height, int depth) {
        // map normalized device coordinates to screen coordinates:
        //  x ∈ [-1, 1] -> [0, width]
        //  y ∈ [-1, 1] -> [0, height]
        //  z ∈ [-1, 1] -> [0, depth]
        return viewport(0, 0, width, height, 0, depth);
    }

    Mat4f look_at(const Vec3f &eye, const Vec3f &target, const Vec3f &up) {
        // ref.: http://www.songho.ca/opengl/gl_camera.html#lookat

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

    Types::Mat4f _orthographic(float left, float right, float bottom, float top, float near, float far) {
        // ref.: http://docs.gl/gl3/glOrtho

        assert(near >= 0 && far > 0);
        float x_range = right - left;
        float y_range = top - bottom;
        float z_range = far - near;
        assert(x_range > 0 && y_range > 0 && z_range > 0);
        return Mat4f(
            2 / x_range,     0      ,      0      , -(left + right) / x_range,
                0      , 2 / y_range,      0      , -(bottom + top) / y_range,
                0      ,     0      , -2 / z_range,  -(near + far)  / z_range,
                0      ,     0      ,      0      ,                 1
        );
    }


    /*
     * float left = -right;
     * float bottom = -top;
     * return _orthographic(left, right, bottom, top, near, far);
     */
    Types::Mat4f orthographic(float right, float top, float near, float far) {
        // ref.: http://www.songho.ca/opengl/gl_projectionmatrix.html

        assert(near >= 0 && far > 0 && right > 0 && top > 0);
        float depth = far - near;
        assert(depth > 0);
        return Mat4f(
            1 / right,    0   ,     0     ,             0       ,
               0     , 1 / top,     0     ,             0       ,
               0     ,    0   , -2 / depth,  -(near+far) / depth,
               0     ,    0   ,     0     ,             1
        );

    }

    Types::Mat4f _perspective(float left, float right, float bottom, float top, float near, float far) {
        // ref.: http://docs.gl/gl3/glFrustum

        assert(near >= 0 && far > 0);
        float x_range = right - left;
        float y_range = top - bottom;
        float z_range = far - near;
        assert(x_range > 0 && y_range > 0 && z_range > 0);
        return Mat4f(
            2*near / x_range,        0        , (left+right) / x_range,             0        ,
                   0        , 2*near / y_range, (bottom+top) / y_range,             0        ,
                   0        ,        0        ,  -(near+far) / z_range, -2*near*far / z_range,
                   0        ,        0        ,             -1       ,              0
        );
    }

    /*
     * float half_h = near * std::tan(Math::deg2rad(vfov_deg) / 2);
     * float half_w = half_h * aspect_ratio;
     * return _perspective(-half_w, half_w, -half_h, half_h, near, far);
     */
    Mat4f perspective(float vfov_deg, float aspect_ratio, float near, float far) {
        // ref.: http://www.songho.ca/opengl/gl_projectionmatrix.html

        float vfov_rad = Math::deg2rad(vfov_deg);
        assert(vfov_rad > 0 && aspect_ratio > 0);

        float depth = far - near;
        assert(near >= 0 && far > 0 && depth > 0);

        float y_div_z = std::tan(vfov_rad / 2);
        float x_div_z = aspect_ratio * y_div_z;
        return Mat4f(
            1 / x_div_z,     0      ,         0        ,          0         ,
                0      , 1 / y_div_z,         0        ,          0         ,
                0      ,     0      , -(near+far)/depth, -(2*near*far)/depth,
                0      ,     0      ,        -1        ,          0
        );
    }

    Mat4f projection(float c) {
        return Mat4f(
            1, 0,   0 , 0,
            0, 1,   0 , 0,
            0, 0,   1 , 0,
            0, 0, -1/c, 1
        );
    }
}