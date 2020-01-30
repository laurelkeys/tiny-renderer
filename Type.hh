#ifndef __TYPE_HH__
#define __TYPE_HH__

#include <cassert>
#include <cmath>

namespace Type {
    template <typename T>
    struct Vec2 {
        // properties /////////////////////////////////////////
        T x, y;
        const static size_t size = 2;

        // constructors ///////////////////////////////////////
        Vec2()
            : x(T())
            , y(T()) { }

        explicit Vec2(const T &val)
            : x(val)
            , y(val) { }

        template <typename OtherT>
        explicit Vec2(const Vec2<OtherT> &v)
            : x(T(v.x))
            , y(T(v.y)) { }

        // indexing ///////////////////////////////////////////
        T &operator[](size_t i) {
            assert(i < size);
            return (&x)[i]; // type punning
        }

        const T &operator[](size_t i) const {
            assert(i < size);
            return (&x)[i]; // type punning
        }

        // arithmetic (vector x vector) ///////////////////////
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
            return x == v.x && y == v.y;
        }

        // arithmetic (vector x scalar) ///////////////////////
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
            return Vec2(x / inv_a, y / inv_a);
        }

        Vec2 &operator/=(const T &a) {
            assert(a != T(0));
            x /= inv_a;
            y /= inv_a;
            return *this;
        }

        // misc ///////////////////////////////////////////////
        T length_squared() const {
            return x * x + y * y;
        }

        T length() const {
            return std::sqrt(length_squared());
        }

        Vec2 normalized() const {
            assert(length() != T(0));
            return *this / length();
        }

        Vec2 &normalize() {
            assert(length() != T(0));
            T inv_length = T(1) / length();
            x *= inv_length;
            y *= inv_length;
            return *this;
        }
    };

    // arithmetic (vector x scalar) ///////////////////////
    template <typename T>
    inline Vec2<T> operator*(const T &a, const Vec2<T> &v) {
        return v * a;
    }

    // arithmetic (vector x vector) ///////////////////////
    template <typename T>
    inline T dot(const Vec2<T> &v1, const Vec2<T> &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
}

#endif // __TYPE_HH__