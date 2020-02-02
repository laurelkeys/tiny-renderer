#ifndef __TYPES_HH__
#define __TYPES_HH__

#include <cassert>
#include <cmath>
#include <iostream>

namespace Types {

    ///////////////////////////////////////////////////////
    /// Vec2 //////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    template <typename T>
    struct Vec2 {
        /// properties ////////////////////////////////////////

        T x, y;
        static const unsigned int size = 2;

        /// constructors //////////////////////////////////////

        Vec2()
            : x(T())
            , y(T()) { }

        Vec2(T x, T y)
            : x(x)
            , y(y) { }

        explicit Vec2(const T &a)
            : x(a)
            , y(a) { }

        template <typename OtherT>
        explicit Vec2(const Vec2<OtherT> &v)
            : x(T(v.x))
            , y(T(v.y)) { }

        /// indexing //////////////////////////////////////////

        T &operator[](unsigned int i) {
            assert(i < size);
            return (&x)[i]; // type punning
        }

        const T &operator[](unsigned int i) const {
            assert(i < size);
            return (&x)[i]; // type punning
        }

        /// arithmetic (vector x vector) //////////////////////

        Vec2 operator-() const {
            return Vec2(-x, -y);
        }

        Vec2 operator+(const Vec2 &v) const {
            return Vec2(x + v.x, y + v.y);
        }

        Vec2 operator-(const Vec2 &v) const {
            return Vec2(x - v.x, y - v.y);
        }

        Vec2 &operator+=(const Vec2 &v) {
            x += v.x;
            y += v.y;
            return *this;
        }

        Vec2 &operator-=(const Vec2 &v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        bool operator==(const Vec2 &v) const {
            return v.x == x && v.y == y;
        }

        bool operator!=(const Vec2 &v) const {
            return v.x != x || v.y != y;
        }

        inline T dot(const Vec2 &v) const {
            return x * v.x + y * v.y;
        }

        /// arithmetic (vector x scalar) //////////////////////

        Vec2 operator*(const T &a) const {
            return Vec2(x * a, y * a);
        }

        Vec2 &operator*=(const T &a) {
            x *= a;
            y *= a;
            return *this;
        }

        Vec2 operator/(const T &a) const {
            assert(a != T(0));
            return Vec2(x / a, y / a);
        }

        Vec2 &operator/=(const T &a) {
            assert(a != T(0));
            x /= a;
            y /= a;
            return *this;
        }

        /// misc //////////////////////////////////////////////

        T length_squared() const {
            return x * x + y * y;
        }

        T length() const {
            return std::sqrt(length_squared());
        }

        Vec2 normalized() const {
            assert(length() != T(0));
            T inv_length = T(1) / length();
            return *this * inv_length;
        }

        Vec2 &normalize() {
            assert(length() != T(0));
            T inv_length = T(1) / length();
            x *= inv_length;
            y *= inv_length;
            return *this;
        }
    };

    /// arithmetic (vector x vector) //////////////////////

    template <typename T>
    inline T dot(const Vec2<T> &v1, const Vec2<T> &v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    /// arithmetic (vector x scalar) //////////////////////

    template <typename T>
    inline Vec2<T> operator*(const T &a, const Vec2<T> &v) {
        return v * a;
    }

    /// misc //////////////////////////////////////////////

    template <typename T>
    std::ostream &operator<<(std::ostream &out, const Vec2<T> &v) {
        out << "(" << v.x << ", " << v.y << ")";
        return out;
    }

    ///////////////////////////////////////////////////////
    /// Vec3 //////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    template <typename T>
    struct Vec3 {
        /// properties ////////////////////////////////////////

        T x, y, z;
        static const unsigned int size = 3;

        /// constructors //////////////////////////////////////

        Vec3()
            : x(T())
            , y(T())
            , z(T()) { }

        Vec3(T x, T y, T z)
            : x(x)
            , y(y)
            , z(z) { }

        explicit Vec3(const T &a)
            : x(a)
            , y(a)
            , z(a) { }

        template <typename OtherT>
        explicit Vec3(const Vec3<OtherT> &v)
            : x(T(v.x))
            , y(T(v.y))
            , z(T(v.z)) { }

        /// indexing //////////////////////////////////////////

        T &operator[](unsigned int i) {
            assert(i < size);
            return (&x)[i]; // type punning
        }

        const T &operator[](unsigned int i) const {
            assert(i < size);
            return (&x)[i]; // type punning
        }

        /// arithmetic (vector x vector) //////////////////////

        Vec3 operator-() const {
            return Vec3(-x, -y, -z);
        }

        Vec3 operator+(const Vec3 &v) const {
            return Vec3(x + v.x, y + v.y, z + v.z);
        }

        Vec3 operator-(const Vec3 &v) const {
            return Vec3(x - v.x, y - v.y, z - v.z);
        }

        Vec3 &operator+=(const Vec3 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        Vec3 &operator-=(const Vec3 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            return *this;
        }

        bool operator==(const Vec3 &v) const {
            return v.x == x && v.y == y && v.z == z;
        }

        bool operator!=(const Vec3 &v) const {
            return v.x != x || v.y != y || v.z != z;
        }

        inline T dot(const Vec3 &v) const {
            return x * v.x + y * v.y + z * v.z;
        }

        inline Vec3 cross(const Vec3 &v) const {
            return Vec3(
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            );
        }

        /// arithmetic (vector x scalar) //////////////////////

        Vec3 operator*(const T &a) const {
            return Vec3(x * a, y * a, z * a);
        }

        Vec3 &operator*=(const T &a) {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }

        Vec3 operator/(const T &a) const {
            assert(a != T(0));
            return Vec3(x / a, y / a, z / a);
        }

        Vec3 &operator/=(const T &a) {
            assert(a != T(0));
            x /= a;
            y /= a;
            z /= a;
            return *this;
        }

        /// misc //////////////////////////////////////////////

        T length_squared() const {
            return x * x + y * y + z * z;
        }

        T length() const {
            return std::sqrt(length_squared());
        }

        Vec3 normalized() const {
            assert(length() != T(0));
            T inv_length = T(1) / length();
            return *this * inv_length;
        }

        Vec3 &normalize() {
            assert(length() != T(0));
            T inv_length = T(1) / length();
            x *= inv_length;
            y *= inv_length;
            z *= inv_length;
            return *this;
        }

        Vec2<T> xy() const {
            return Vec2<T>(x, y);
        }
    };

    /// arithmetic (vector x vector) //////////////////////

    template <typename T>
    inline T dot(const Vec3<T> &v1, const Vec3<T> &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template <typename T>
    inline Vec3<T> cross(const Vec3<T> &v1, const Vec3<T> &v2) {
        return Vec3<T>(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    /// arithmetic (vector x scalar) //////////////////////

    template <typename T>
    inline Vec3<T> operator*(const T &a, const Vec3<T> &v) {
        return v * a;
    }

    /// misc //////////////////////////////////////////////

    template <typename T>
    std::ostream &operator<<(std::ostream &out, const Vec3<T> &v) {
        out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return out;
    }

    ///////////////////////////////////////////////////////
    /// Vec4 //////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    template <typename T>
    struct Vec4 {
        /// properties ////////////////////////////////////////

        T x, y, z, w;
        static const unsigned int size = 4;

        /// constructors //////////////////////////////////////

        Vec4()
            : x(T())
            , y(T())
            , z(T())
            , w(T()) { }

        Vec4(T x, T y, T z, T w)
            : x(x)
            , y(y)
            , z(z)
            , w(w) { }

        explicit Vec4(const T &a)
            : x(a)
            , y(a)
            , z(a)
            , w(a) { }

        template <typename OtherT>
        explicit Vec4(const Vec4<OtherT> &v)
            : x(T(v.x))
            , y(T(v.y))
            , z(T(v.z))
            , w(T(v.w)) { }

        /// indexing //////////////////////////////////////////

        T &operator[](unsigned int i) {
            assert(i < size);
            return (&x)[i]; // type punning
        }

        const T &operator[](unsigned int i) const {
            assert(i < size);
            return (&x)[i]; // type punning
        }

        /// arithmetic (vector x vector) //////////////////////

        Vec4 operator-() const {
            return Vec4(-x, -y, -z, -w);
        }

        Vec4 operator+(const Vec4 &v) const {
            return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
        }

        Vec4 operator-(const Vec4 &v) const {
            return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
        }

        Vec4 &operator+=(const Vec4 &v) {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        }

        Vec4 &operator-=(const Vec4 &v) {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            return *this;
        }

        bool operator==(const Vec4 &v) const {
            return v.x == x && v.y == y && v.z == z && v.w == w;
        }

        bool operator!=(const Vec4 &v) const {
            return v.x != x || v.y != y || v.z != z || v.w != w;
        }

        inline T dot(const Vec4 &v) {
            return x * v.x + y * v.y + z * v.z + w * v.w;
        }

        /// arithmetic (vector x scalar) //////////////////////

        Vec4 operator*(const T &a) const {
            return Vec4(x * a, y * a, z * a, w * a);
        }

        Vec4 &operator*=(const T &a) {
            x *= a;
            y *= a;
            z *= a;
            w *= a;
            return *this;
        }

        Vec4 operator/(const T &a) const {
            assert(a != T(0));
            return Vec4(x / a, y / a, z / a, w / a);
        }

        Vec4 &operator/=(const T &a) {
            assert(a != T(0));
            x /= a;
            y /= a;
            z /= a;
            w /= a;
            return *this;
        }

        /// misc //////////////////////////////////////////////

        T length_squared() const {
            return x * x + y * y + z * z + w * w;
        }

        T length() const {
            return std::sqrt(length_squared());
        }

        Vec4 normalized() const {
            assert(length() != T(0));
            T inv_length = T(1) / length();
            return *this * inv_length;
        }

        Vec4 &normalize() {
            assert(length() != T(0));
            T inv_length = T(1) / length();
            x *= inv_length;
            y *= inv_length;
            z *= inv_length;
            w *= inv_length;
            return *this;
        }

        Vec4 &homogenize() {
            assert(w != T(0));
            T inv_w = T(1) / w;
            x *= inv_w;
            y *= inv_w;
            z *= inv_w;
            w = T(1); // w *= inv_w;
            return *this;
        }
    };

    /// arithmetic (vector x vector) //////////////////////

    template <typename T>
    inline T dot(const Vec4<T> &v1, const Vec4<T> &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    /// arithmetic (vector x scalar) //////////////////////

    template <typename T>
    inline Vec4<T> operator*(const T &a, const Vec4<T> &v) {
        return v * a;
    }

    /// misc //////////////////////////////////////////////

    template <typename T>
    std::ostream &operator<<(std::ostream &out, const Vec4<T> &v) {
        out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        return out;
    }

    ///////////////////////////////////////////////////////
    /// typedefs //////////////////////////////////////////
    ///////////////////////////////////////////////////////

    typedef Vec2<int> Vec2i;
    typedef Vec3<int> Vec3i;
    typedef Vec4<int> Vec4i;

    typedef Vec2<float> Vec2f;
    typedef Vec3<float> Vec3f;
    typedef Vec4<float> Vec4f;

    typedef Vec2<unsigned int> Vec2u;
    typedef Vec3<unsigned int> Vec3u;
    typedef Vec4<unsigned int> Vec4u;

    ///////////////////////////////////////////////////////
    /// Mat4f /////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    struct Mat4f {
        /// properties ////////////////////////////////////////

        union {
            struct {
                float _11, _12, _13, _14;
                float _21, _22, _23, _24;
                float _31, _32, _33, _34;
                float _41, _42, _43, _44;
            };
            float _m[16];
        };

        /// constructors //////////////////////////////////////

        // Creates an identity matrix
        Mat4f()
            : _11(1), _12(0), _13(0), _14(0)
            , _21(0), _22(1), _23(0), _24(0)
            , _31(0), _32(0), _33(1), _34(0)
            , _41(0), _42(0), _43(0), _44(1) { }

        Mat4f(float a11, float a12, float a13, float a14,
              float a21, float a22, float a23, float a24,
              float a31, float a32, float a33, float a34,
              float a41, float a42, float a43, float a44)
            : _11(a11), _12(a12), _13(a13), _14(a14)
            , _21(a21), _22(a22), _23(a23), _24(a24)
            , _31(a31), _32(a32), _33(a33), _34(a34)
            , _41(a41), _42(a42), _43(a43), _44(a44) { }

        explicit Mat4f(float a)
            : _11(a), _12(a), _13(a), _14(a)
            , _21(a), _22(a), _23(a), _24(a)
            , _31(a), _32(a), _33(a), _34(a)
            , _41(a), _42(a), _43(a), _44(a) { }

        Mat4f(const Mat4f &m)
            : _11(m._11), _12(m._12), _13(m._13), _14(m._14)
            , _21(m._21), _22(m._22), _23(m._23), _24(m._24)
            , _31(m._31), _32(m._32), _33(m._33), _34(m._34)
            , _41(m._41), _42(m._42), _43(m._43), _44(m._44) { }

        /// indexing //////////////////////////////////////////

        // i-th row, starting at 0 (ending at 3)
        Vec4f row(unsigned int i) const {
            assert(i < 4);
            return Vec4f(
                _m[4 * i + 0],
                _m[4 * i + 1],
                _m[4 * i + 2],
                _m[4 * i + 3]
            );
        }

        void set_row(unsigned int i, const Vec4f &v) {
            assert(i < 4);
            _m[4 * i + 0] = v.x;
            _m[4 * i + 1] = v.y;
            _m[4 * i + 2] = v.z;
            _m[4 * i + 3] = v.w;
        }
        void set_row(unsigned int i, const Vec3f &v) {
            assert(i < 4);
            _m[4 * i + 0] = v.x;
            _m[4 * i + 1] = v.y;
            _m[4 * i + 2] = v.z;
        }

        // j-th column, starting at 0 (ending at 3)
        Vec4f col(unsigned int j) const {
            assert(j < 4);
            return Vec4f(
                _m[4 * 0 + j],
                _m[4 * 1 + j],
                _m[4 * 2 + j],
                _m[4 * 3 + j]
            );
        }

        void set_col(unsigned int j, const Vec4f &v) {
            assert(j < 4);
            _m[4 * 0 + j] = v.x;
            _m[4 * 1 + j] = v.y;
            _m[4 * 2 + j] = v.z;
            _m[4 * 3 + j] = v.w;
        }
        void set_col(unsigned int j, const Vec3f &v) {
            assert(j < 4);
            _m[4 * 0 + j] = v.x;
            _m[4 * 1 + j] = v.y;
            _m[4 * 2 + j] = v.z;
        }

        /// arithmetic (matrix x scalar) //////////////////////

        Mat4f operator*(float a) const {
            return Mat4f(
                _11 * a, _12 * a, _13 * a, _14 * a,
                _21 * a, _22 * a, _23 * a, _24 * a,
                _31 * a, _32 * a, _33 * a, _34 * a,
                _41 * a, _42 * a, _43 * a, _44 * a
            );
        }

        Mat4f &operator*=(float a) {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    _m[4 * i + j] = a;
            return *this;
        }

        /// arithmetic (matrix x vector) //////////////////////

        Vec4f operator*(const Vec4f &v) const {
            return Vec4f(
                _11 * v.x + _12 * v.y + _13 * v.z + _14 * v.w,
                _21 * v.x + _22 * v.y + _23 * v.z + _24 * v.w,
                _31 * v.x + _32 * v.y + _33 * v.z + _34 * v.w,
                _41 * v.x + _42 * v.y + _43 * v.z + _44 * v.w
            );
        }
        Vec3f operator*(const Vec3f &v) const {
            // "v.w == 1"
            return Vec3f(
                _11 * v.x + _12 * v.y + _13 * v.z + _14,
                _21 * v.x + _22 * v.y + _23 * v.z + _24,
                _31 * v.x + _32 * v.y + _33 * v.z + _34
            );
        }

        /// arithmetic (matrix x matrix) //////////////////////

        Mat4f operator*(const Mat4f &m) const {
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

        bool operator==(const Mat4f &m) const {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if (m._m[4 * i + j] != _m[4 * i + j])
                        return false;
            return true;
        }

        bool operator!=(const Mat4f &m) const {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    if (m._m[4 * i + j] != _m[4 * i + j])
                        return true;
            return false;
        }

        /// constants /////////////////////////////////////////

        static const Mat4f identity() {
            return Mat4f(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            );
        }

        static const Mat4f zero() {
            return Mat4f(
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0
            );
        }

        /// misc //////////////////////////////////////////////

        // Returns the determinant of the upper left 3 x 3 submatrix
        inline float det3x3() const {
            return (
                  _11 * (_22 * _33 - _23 * _32)
                - _12 * (_21 * _33 - _23 * _31)
                + _13 * (_21 * _32 - _22 * _31)
            );
        }

        Mat4f transposed() const {
            return Mat4f(
                _11, _21, _31, _41,
                _12, _22, _32, _42,
                _13, _23, _33, _43,
                _14, _24, _34, _44
            );
        }

        friend std::ostream &operator<<(std::ostream &out, const Mat4f &m) {
            out << m.row(0) << std::endl; // (_11, _12, _13, _14)
            out << m.row(1) << std::endl; // (_21, _22, _23, _24)
            out << m.row(2) << std::endl; // (_31, _32, _33, _34)
            out << m.row(3) << std::endl; // (_41, _42, _43, _44)
            return out;
        }
    };

    /// arithmetic (matrix x scalar) //////////////////////

    inline Mat4f operator*(float a, const Mat4f &m) {
        return m * a;
    }
}

#endif // __TYPES_HH__