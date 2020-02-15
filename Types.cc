#include "Types.hh"

namespace Types {

    /// float to int conversion ///////////////////////////

    template <>
    template <>
    Vec2<int>::Vec2(const Vec2<float> &v)
        : x(int(v.x + .5f))
        , y(int(v.y + .5f)) { }

    template <>
    template <>
    Vec3<int>::Vec3(const Vec3<float> &v)
        : x(int(v.x + .5f))
        , y(int(v.y + .5f))
        , z(int(v.z + .5f)) { }

    template <>
    template <>
    Vec4<int>::Vec4(const Vec4<float> &v)
        : x(int(v.x + .5f))
        , y(int(v.y + .5f))
        , z(int(v.z + .5f))
        , w(int(v.w + .5f)) { }

    /// int to float conversion ///////////////////////////

    template <>
    template <>
    Vec2<float>::Vec2(const Vec2<int> &v)
        : x(v.x)
        , y(v.y) { }

    template <>
    template <>
    Vec3<float>::Vec3(const Vec3<int> &v)
        : x(v.x)
        , y(v.y)
        , z(v.z) { }

    template <>
    template <>
    Vec4<float>::Vec4(const Vec4<int> &v)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(v.w) { }
}