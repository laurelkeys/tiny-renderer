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

    Types::Mat4f viewport(int leftmost_x, int bottommost_y, int width, int height);
    Types::Mat4f viewport(int width, int height); // viewport(0, 0, width, height)

    Types::Mat4f look_at(const Types::Vec3f &eye, const Types::Vec3f &target, const Types::Vec3f &up);
    
    // Types::Mat4f perspective(float vfov_deg, float aspect_ratio, float near, float far);
    Types::Mat4f projection(float c); // obs.: assumes a camera at (0, 0, c)
}

#endif // __TRASNSFORM_HH__