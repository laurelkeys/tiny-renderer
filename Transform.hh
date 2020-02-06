#ifndef __TRASNSFORM_HH__
#define __TRASNSFORM_HH__

#include "Types.hh"

namespace Transform {

    static Types::Mat4f translate(const Types::Vec3f &v);
    
    static Types::Mat4f scale(const Types::Vec3f &v);
    
    static Types::Mat4f rotate(float angle_deg, const Types::Vec3f &axis);
    static Types::Mat4f rotate_x(float angle_deg);
    static Types::Mat4f rotate_y(float angle_deg);
    static Types::Mat4f rotate_z(float angle_deg);
}

#endif // __TRASNSFORM_HH__