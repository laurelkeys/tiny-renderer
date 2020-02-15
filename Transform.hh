#ifndef __TRASNSFORM_HH__
#define __TRASNSFORM_HH__

#include "Types.hh"

namespace Transform {

    Types::Mat4f translate(const Types::Vec3f &v);

    Types::Mat4f scale(const Types::Vec3f &v);
    Types::Mat4f scale(float a); // scale(Vec3f(a, a, a))

    Types::Mat4f rotate(float angle_deg, const Types::Vec3f &axis);
    Types::Mat4f rotate_x(float angle_deg);
    Types::Mat4f rotate_y(float angle_deg);
    Types::Mat4f rotate_z(float angle_deg);

    Types::Mat4f viewport(int min_x, int min_y, int width, int height,
                          int near = 0, int far = 255);
    Types::Mat4f viewport(int width, int height, int depth = 255); // viewport(0, 0, width, height, 0, depth)

    Types::Mat4f look_at(const Types::Vec3f &eye, const Types::Vec3f &target, const Types::Vec3f &up);

    // Types::Mat4f perspective(float vfov_deg, float aspect_ratio, float near, float far);
    Types::Mat4f projection(float c); // obs.: assumes a camera at (0, 0, c)
}

#endif // __TRASNSFORM_HH__