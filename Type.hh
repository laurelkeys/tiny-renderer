#ifndef __TYPE_HH__
#define __TYPE_HH__

#include <cassert>
#include <cmath>

namespace Type {

    ///////////////////////////////////////////////////////
    /// Vec2 //////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    template <typename T>
    struct Vec2 {
        /// properties ////////////////////////////////////////
        T x, y;
        const static unsigned int size = 2;

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

        friend std::ostream &operator<<(std::ostream &out, Vec2 &v) {
            out << "(" << x << ", " << y << ")";
            return out;
        }
    };

    /// arithmetic (vector x scalar) //////////////////////
    template <typename T>
    inline Vec2<T> operator*(const T &a, const Vec2<T> &v) {
        return v * a;
    }

    /// arithmetic (vector x vector) //////////////////////
    template <typename T>
    inline T dot(const Vec2<T> &v1, const Vec2<T> &v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    ///////////////////////////////////////////////////////
    /// Vec3 //////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    template <typename T>
    struct Vec3 {
        /// properties ////////////////////////////////////////
        T x, y, z;
        const static unsigned int size = 3;

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

        friend std::ostream &operator<<(std::ostream &out, Vec3 &v) {
            out << "(" << x << ", " << y << ", " << z << ")";
            return out;
        }
    };

    /// arithmetic (vector x scalar) //////////////////////
    template <typename T>
    inline Vec3<T> operator*(const T &a, const Vec3<T> &v) {
        return v * a;
    }

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

    ///////////////////////////////////////////////////////
    /// Vec4 //////////////////////////////////////////////
    ///////////////////////////////////////////////////////
    template <typename T>
    struct Vec4 {
        /// properties ////////////////////////////////////////
        T x, y, z, w;
        const static unsigned int size = 4;

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
            return x * x + y * y + z * z + ;
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

        friend std::ostream &operator<<(std::ostream &out, Vec3 &v) {
            out << "(" << x << ", " << y << ", " << z << ", " << w << ")";
            return out;
        }
    };

    /// arithmetic (vector x scalar) //////////////////////
    template <typename T>
    inline Vec4<T> operator*(const T &a, const Vec4<T> &v) {
        return v * a;
    }

    /// arithmetic (vector x vector) //////////////////////
    template <typename T>
    inline T dot(const Vec4<T> &v1, const Vec4<T> &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    ///////////////////////////////////////////////////////
    /// typedefs //////////////////////////////////////////
    ///////////////////////////////////////////////////////
    typedef Vec4<int> Vec4i;
    typedef Vec3<int> Vec3i;
    typedef Vec2<int> Vec2i;

    typedef Vec4<float> Vec4f;
    typedef Vec3<float> Vec3f;
    typedef Vec2<float> Vec2f;

    typedef Vec4<unsigned int> Vec4u;
    typedef Vec3<unsigned int> Vec3u;
    typedef Vec2<unsigned int> Vec2u;
}

#endif // __TYPE_HH__