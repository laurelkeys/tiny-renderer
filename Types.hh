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
        Vec2(const Vec2<OtherT> &v);

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
        Vec3(const Vec3<OtherT> &v);

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

        explicit Vec4(const Vec3<T> &v, T w)
            : x(v.x)
            , y(v.y)
            , z(v.z)
            , w(w) { }

        template <typename OtherT>
        Vec4(const Vec4<OtherT> &v);

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

        Vec3<T> xyz() const {
            return Vec3<T>(x, y, z);
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
    /// Mat3f /////////////////////////////////////////////
    ///////////////////////////////////////////////////////

    struct Mat3f {
        /// properties ////////////////////////////////////////

        union {
            struct {
                float _11, _12, _13;
                float _21, _22, _23;
                float _31, _32, _33;
            };
            float _m[9];
        };

        /// constructors //////////////////////////////////////

        // Creates an identity matrix
        Mat3f();

        Mat3f(float a11, float a12, float a13,
              float a21, float a22, float a23,
              float a31, float a32, float a33);

        explicit Mat3f(float a);

        Mat3f(const Mat3f &m);

        /// indexing //////////////////////////////////////////

        // i-th row, starting at 0 (ending at 2)
        Vec3f row(unsigned int i) const;

        void set_row(unsigned int i, const Vec3f &v);

        // j-th column, starting at 0 (ending at 2)
        Vec3f col(unsigned int j) const;

        void set_col(unsigned int j, const Vec3f &v);

        // i-th row, j-th column, starting at 0 (ending at 2)
        float cell(unsigned int i, unsigned int j) const;

        void set_cell(unsigned int i, unsigned int j, float a);

        /// arithmetic (matrix x scalar) //////////////////////

        Mat3f operator*(float a) const;

        Mat3f &operator*=(float a);

        /// arithmetic (matrix x vector) //////////////////////

        Vec3f operator*(const Vec3f &v) const;

        /// arithmetic (matrix x matrix) //////////////////////

        Mat3f operator*(const Mat3f &m) const;

        bool operator==(const Mat3f &m) const;

        bool operator!=(const Mat3f &m) const;

        /// constants /////////////////////////////////////////

        static const Mat3f identity() {
            return Mat3f(
                1, 0, 0,
                0, 1, 0,
                0, 0, 1
            );
        }

        static const Mat3f zero() {
            return Mat3f(
                0, 0, 0,
                0, 0, 0,
                0, 0, 0
            );
        }

        /// misc //////////////////////////////////////////////

        inline float det3x3() const;

        Mat3f transposed() const;

        Mat3f inversed() const;

        friend std::ostream &operator<<(std::ostream &out, const Mat3f &m);
    };

    /// arithmetic (matrix x scalar) //////////////////////

    inline Mat3f operator*(float a, const Mat3f &m);

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
        Mat4f();

        Mat4f(float a11, float a12, float a13, float a14,
              float a21, float a22, float a23, float a24,
              float a31, float a32, float a33, float a34,
              float a41, float a42, float a43, float a44);

        explicit Mat4f(float a);

        Mat4f(const Mat4f &m);

        /// indexing //////////////////////////////////////////

        // i-th row, starting at 0 (ending at 3)
        Vec4f row(unsigned int i) const;

        void set_row(unsigned int i, const Vec4f &v);
        void set_row(unsigned int i, const Vec3f &v);

        // j-th column, starting at 0 (ending at 3)
        Vec4f col(unsigned int j) const;

        void set_col(unsigned int j, const Vec4f &v);
        void set_col(unsigned int j, const Vec3f &v);

        // i-th row, j-th column, starting at 0 (ending at 3)
        float cell(unsigned int i, unsigned int j) const;

        void set_cell(unsigned int i, unsigned int j, float a);

        /// arithmetic (matrix x scalar) //////////////////////

        Mat4f operator*(float a) const;

        Mat4f &operator*=(float a);

        /// arithmetic (matrix x vector) //////////////////////

        Vec4f operator*(const Vec4f &v) const;
        Vec3f operator*(const Vec3f &v) const;

        /// arithmetic (matrix x matrix) //////////////////////

        Mat4f operator*(const Mat4f &m) const;

        bool operator==(const Mat4f &m) const;

        bool operator!=(const Mat4f &m) const;

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
        inline float det3x3() const;

        Mat4f transposed() const;

        Mat4f inversed() const;

        friend std::ostream &operator<<(std::ostream &out, const Mat4f &m);
    };

    /// arithmetic (matrix x scalar) //////////////////////

    inline Mat4f operator*(float a, const Mat4f &m);
}

#endif // __TYPES_HH__