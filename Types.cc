#include "Types.hh"

namespace Types {

    ///////////////////////////////////////////////////////
    /// template specialization ///////////////////////////
    ///////////////////////////////////////////////////////

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

    ///////////////////////////////////////////////////////
    /// Mat4f /////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    /// constructors //////////////////////////////////////

    // Creates an identity matrix
    Mat4f::Mat4f()
        : _11(1), _12(0), _13(0), _14(0)
        , _21(0), _22(1), _23(0), _24(0)
        , _31(0), _32(0), _33(1), _34(0)
        , _41(0), _42(0), _43(0), _44(1) { }

    Mat4f::Mat4f(float a11, float a12, float a13, float a14,
            float a21, float a22, float a23, float a24,
            float a31, float a32, float a33, float a34,
            float a41, float a42, float a43, float a44)
        : _11(a11), _12(a12), _13(a13), _14(a14)
        , _21(a21), _22(a22), _23(a23), _24(a24)
        , _31(a31), _32(a32), _33(a33), _34(a34)
        , _41(a41), _42(a42), _43(a43), _44(a44) { }

    Mat4f::Mat4f(float a)
        : _11(a), _12(a), _13(a), _14(a)
        , _21(a), _22(a), _23(a), _24(a)
        , _31(a), _32(a), _33(a), _34(a)
        , _41(a), _42(a), _43(a), _44(a) { }

    Mat4f::Mat4f(const Mat4f &m)
        : _11(m._11), _12(m._12), _13(m._13), _14(m._14)
        , _21(m._21), _22(m._22), _23(m._23), _24(m._24)
        , _31(m._31), _32(m._32), _33(m._33), _34(m._34)
        , _41(m._41), _42(m._42), _43(m._43), _44(m._44) { }

    /// indexing //////////////////////////////////////////

    // i-th row, starting at 0 (ending at 3)
    Vec4f Mat4f::row(unsigned int i) const {
        assert(i < 4);
        return Vec4f(
            _m[4 * i + 0],
            _m[4 * i + 1],
            _m[4 * i + 2],
            _m[4 * i + 3]
        );
    }

    void Mat4f::set_row(unsigned int i, const Vec4f &v) {
        assert(i < 4);
        _m[4 * i + 0] = v.x;
        _m[4 * i + 1] = v.y;
        _m[4 * i + 2] = v.z;
        _m[4 * i + 3] = v.w;
    }
    void Mat4f::set_row(unsigned int i, const Vec3f &v) {
        assert(i < 4);
        _m[4 * i + 0] = v.x;
        _m[4 * i + 1] = v.y;
        _m[4 * i + 2] = v.z;
    }

    // j-th column, starting at 0 (ending at 3)
    Vec4f Mat4f::col(unsigned int j) const {
        assert(j < 4);
        return Vec4f(
            _m[4 * 0 + j],
            _m[4 * 1 + j],
            _m[4 * 2 + j],
            _m[4 * 3 + j]
        );
    }

    void Mat4f::set_col(unsigned int j, const Vec4f &v) {
        assert(j < 4);
        _m[4 * 0 + j] = v.x;
        _m[4 * 1 + j] = v.y;
        _m[4 * 2 + j] = v.z;
        _m[4 * 3 + j] = v.w;
    }
    void Mat4f::set_col(unsigned int j, const Vec3f &v) {
        assert(j < 4);
        _m[4 * 0 + j] = v.x;
        _m[4 * 1 + j] = v.y;
        _m[4 * 2 + j] = v.z;
    }

    // i-th row, j-th column, starting at 0 (ending at 3)
    float Mat4f::cell(unsigned int i, unsigned int j) const {
        assert(i < 4 && j < 4);
        return _m[4 * i + j];
    }

    void Mat4f::set_cell(unsigned int i, unsigned int j, float a) {
        assert(i < 4 && j < 4);
        _m[4 * i + j] = a;
    }

    /// arithmetic (matrix x scalar) //////////////////////

    Mat4f Mat4f::operator*(float a) const {
        return Mat4f(
            _11 * a, _12 * a, _13 * a, _14 * a,
            _21 * a, _22 * a, _23 * a, _24 * a,
            _31 * a, _32 * a, _33 * a, _34 * a,
            _41 * a, _42 * a, _43 * a, _44 * a
        );
    }

    Mat4f &Mat4f::operator*=(float a) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                _m[4 * i + j] = a;
        return *this;
    }

    /// arithmetic (matrix x vector) //////////////////////

    Vec4f Mat4f::operator*(const Vec4f &v) const {
        return Vec4f(
            _11 * v.x + _12 * v.y + _13 * v.z + _14 * v.w,
            _21 * v.x + _22 * v.y + _23 * v.z + _24 * v.w,
            _31 * v.x + _32 * v.y + _33 * v.z + _34 * v.w,
            _41 * v.x + _42 * v.y + _43 * v.z + _44 * v.w
        );
    }
    Vec3f Mat4f::operator*(const Vec3f &v) const {
        // "v.w == 1"
        return Vec3f(
            _11 * v.x + _12 * v.y + _13 * v.z + _14,
            _21 * v.x + _22 * v.y + _23 * v.z + _24,
            _31 * v.x + _32 * v.y + _33 * v.z + _34
        );
    }

    /// arithmetic (matrix x matrix) //////////////////////

    Mat4f Mat4f::operator*(const Mat4f &m) const {
        Mat4f result;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                result._m[4 * i + j] = (
                    _m[4 * i + 0] * m._m[4 * 0 + j] +
                    _m[4 * i + 1] * m._m[4 * 1 + j] +
                    _m[4 * i + 2] * m._m[4 * 2 + j] +
                    _m[4 * i + 3] * m._m[4 * 3 + j]
                );
        return result;
    }

    bool Mat4f::operator==(const Mat4f &m) const {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (m._m[4 * i + j] != _m[4 * i + j])
                    return false;
        return true;
    }

    bool Mat4f::operator!=(const Mat4f &m) const {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (m._m[4 * i + j] != _m[4 * i + j])
                    return true;
        return false;
    }

    /// misc //////////////////////////////////////////////

    // Returns the determinant of the upper left 3 x 3 submatrix
    inline float Mat4f::det3x3() const {
        return (
              _11 * (_22 * _33 - _23 * _32)
            - _12 * (_21 * _33 - _23 * _31)
            + _13 * (_21 * _32 - _22 * _31)
        );
    }

    Mat4f Mat4f::transposed() const {
        return Mat4f(
            _11, _21, _31, _41,
            _12, _22, _32, _42,
            _13, _23, _33, _43,
            _14, _24, _34, _44
        );
    }

    Mat4f Mat4f::transposed() const {
        // ref.: https://github.com/tunabrain/tungsten/blob/master/src/core/math/Mat4f.hpp
        
        Mat4f inv;
        inv._m[ 0] =  _m[5]*_m[10]*_m[15] - _m[5]*_m[11]*_m[14] - _m[9]*_m[6]*_m[15] + _m[9]*_m[7]*_m[14] + _m[13]*_m[6]*_m[11] - _m[13]*_m[7]*_m[10];
        inv._m[ 1] = -_m[1]*_m[10]*_m[15] + _m[1]*_m[11]*_m[14] + _m[9]*_m[2]*_m[15] - _m[9]*_m[3]*_m[14] - _m[13]*_m[2]*_m[11] + _m[13]*_m[3]*_m[10];
        inv._m[ 2] =  _m[1]*_m[ 6]*_m[15] - _m[1]*_m[ 7]*_m[14] - _m[5]*_m[2]*_m[15] + _m[5]*_m[3]*_m[14] + _m[13]*_m[2]*_m[ 7] - _m[13]*_m[3]*_m[ 6];
        inv._m[ 3] = -_m[1]*_m[ 6]*_m[11] + _m[1]*_m[ 7]*_m[10] + _m[5]*_m[2]*_m[11] - _m[5]*_m[3]*_m[10] - _m[ 9]*_m[2]*_m[ 7] + _m[ 9]*_m[3]*_m[ 6];
        inv._m[ 4] = -_m[4]*_m[10]*_m[15] + _m[4]*_m[11]*_m[14] + _m[8]*_m[6]*_m[15] - _m[8]*_m[7]*_m[14] - _m[12]*_m[6]*_m[11] + _m[12]*_m[7]*_m[10];
        inv._m[ 5] =  _m[0]*_m[10]*_m[15] - _m[0]*_m[11]*_m[14] - _m[8]*_m[2]*_m[15] + _m[8]*_m[3]*_m[14] + _m[12]*_m[2]*_m[11] - _m[12]*_m[3]*_m[10];
        inv._m[ 6] = -_m[0]*_m[ 6]*_m[15] + _m[0]*_m[ 7]*_m[14] + _m[4]*_m[2]*_m[15] - _m[4]*_m[3]*_m[14] - _m[12]*_m[2]*_m[ 7] + _m[12]*_m[3]*_m[ 6];
        inv._m[ 8] =  _m[4]*_m[ 9]*_m[15] - _m[4]*_m[11]*_m[13] - _m[8]*_m[5]*_m[15] + _m[8]*_m[7]*_m[13] + _m[12]*_m[5]*_m[11] - _m[12]*_m[7]*_m[ 9];
        inv._m[ 7] =  _m[0]*_m[ 6]*_m[11] - _m[0]*_m[ 7]*_m[10] - _m[4]*_m[2]*_m[11] + _m[4]*_m[3]*_m[10] + _m[ 8]*_m[2]*_m[ 7] - _m[ 8]*_m[3]*_m[ 6];
        inv._m[ 9] = -_m[0]*_m[ 9]*_m[15] + _m[0]*_m[11]*_m[13] + _m[8]*_m[1]*_m[15] - _m[8]*_m[3]*_m[13] - _m[12]*_m[1]*_m[11] + _m[12]*_m[3]*_m[ 9];
        inv._m[10] =  _m[0]*_m[ 5]*_m[15] - _m[0]*_m[ 7]*_m[13] - _m[4]*_m[1]*_m[15] + _m[4]*_m[3]*_m[13] + _m[12]*_m[1]*_m[ 7] - _m[12]*_m[3]*_m[ 5];
        inv._m[11] = -_m[0]*_m[ 5]*_m[11] + _m[0]*_m[ 7]*_m[ 9] + _m[4]*_m[1]*_m[11] - _m[4]*_m[3]*_m[ 9] - _m[ 8]*_m[1]*_m[ 7] + _m[ 8]*_m[3]*_m[ 5];
        inv._m[12] = -_m[4]*_m[ 9]*_m[14] + _m[4]*_m[10]*_m[13] + _m[8]*_m[5]*_m[14] - _m[8]*_m[6]*_m[13] - _m[12]*_m[5]*_m[10] + _m[12]*_m[6]*_m[ 9];
        inv._m[13] =  _m[0]*_m[ 9]*_m[14] - _m[0]*_m[10]*_m[13] - _m[8]*_m[1]*_m[14] + _m[8]*_m[2]*_m[13] + _m[12]*_m[1]*_m[10] - _m[12]*_m[2]*_m[ 9];
        inv._m[14] = -_m[0]*_m[ 5]*_m[14] + _m[0]*_m[ 6]*_m[13] + _m[4]*_m[1]*_m[14] - _m[4]*_m[2]*_m[13] - _m[12]*_m[1]*_m[ 6] + _m[12]*_m[2]*_m[ 5];
        inv._m[15] =  _m[0]*_m[ 5]*_m[10] - _m[0]*_m[ 6]*_m[ 9] - _m[4]*_m[1]*_m[10] + _m[4]*_m[2]*_m[ 9] + _m[ 8]*_m[1]*_m[ 6] - _m[ 8]*_m[2]*_m[ 5];

        float det = _m[0]*inv._m[0] + _m[1]*inv._m[4] + _m[2]*inv._m[8] + _m[3]*inv._m[12];
        if (det == 0.0f)
            return Mat4f();

        return inv * (1 / det);
    }

    std::ostream &operator<<(std::ostream &out, const Mat4f &m) {
        out << m.row(0) << std::endl; // (_11, _12, _13, _14)
        out << m.row(1) << std::endl; // (_21, _22, _23, _24)
        out << m.row(2) << std::endl; // (_31, _32, _33, _34)
        out << m.row(3) << std::endl; // (_41, _42, _43, _44)
        return out;
    }

    /// arithmetic (matrix x scalar) //////////////////////

    inline Mat4f operator*(float a, const Mat4f &m) {
        return m * a;
    }
}