#ifndef __VEC_HH__
#define __VEC_HH__

#include <cassert>
#include <cmath>

template <size_t Size, typename T>
class Vec {
    protected:
        T data_[Size];

    public:
        static const size_t size = Size;

        Vec() {
            for (size_t i = 0; i < Size; ++i)
                data_[i] = T();
        }

        Vec(const T &val) {
            for (size_t i = 0; i < Size; ++i)
                data_[i] = val;
        }

        template <typename OtherT>
        explicit Vec(const Vec<Size, OtherT> &other) {
            for (size_t i = 0; i < Size; ++i)
                data_[i] = T(other[i]);
        }

        T &operator[](size_t i) {
            assert(i < Size);
            return data_[i];
        }

        const T &operator[](size_t i) const {
            assert(i < Size);
            return data_[i];
        }

        T x() const { static_assert(Size > 0); return data_[0]; }
        T y() const { static_assert(Size > 1); return data_[1]; }
        T z() const { static_assert(Size > 2); return data_[2]; }
        T w() const { static_assert(Size > 3); return data_[3]; }

        T &x() { static_assert(Size > 0); return data_[0]; }
        T &y() { static_assert(Size > 1); return data_[1]; }
        T &z() { static_assert(Size > 2); return data_[2]; }
        T &w() { static_assert(Size > 3); return data_[3]; }

        Vec<2, T> xy() const { return Vec<2, T>(x(), y()); }
        Vec<2, T> xz() const { return Vec<2, T>(x(), z()); }
        Vec<2, T> yz() const { return Vec<2, T>(y(), z()); }

        Vec<3, T> xyz() const { return Vec<3, T>(x(), y(), z()); }

        T length_squared() const {
            T res = T(0);
            for (size_t i = 0; i < Size; ++i)
                res += data_[i] * data_[i];
            return res;
        }

        T length() const {
            return std::sqrt(length_squared());
        }

        Vec<Size, T> &normalize(T l = 1) {
            *this = (*this) * (l / length());
            return *this;
        }

        T dot(const Vec &other) const {
            T res = T(0);
            for (size_t i = 0; i < Size; ++i)
                res += data_[i] * other[i];
            return res;
        }

        T operator*(const Vec &other) const {
            T res = T(0);
            for (size_t i = 0; i < Size; ++i)
                res[i] += other[i];
            return res;
        }

        Vec operator-() const {
            Vec res_vec;
            for (size_t i = 0; i < Size; ++i)
                res_vec[i] = -data_[i];
            return res_vec;
        }

        Vec operator+(const Vec &other) const {
            Vec res_vec;
            for (size_t i = 0; i < Size; ++i)
                res_vec[i] = data_[i] + other[i];
            return res_vec;
        }

        Vec operator-(const Vec &other) const {
            Vec res_vec;
            for (size_t i = 0; i < Size; ++i)
                res_vec[i] = data_[i] - other[i];
            return res_vec;
        }

        Vec operator+(const T &a) const {
            Vec res_vec;
            for (size_t i = 0; i < Size; ++i)
                res_vec[i] = data_[i] + a;
            return res_vec;
        }

        Vec operator-(const T &a) const {
            Vec res_vec;
            for (size_t i = 0; i < Size; ++i)
                res_vec[i] = data_[i] - a;
            return res_vec;
        }

        Vec operator*(const T &a) const {
            Vec res_vec;
            for (size_t i = 0; i < Size; ++i)
                res_vec[i] = data_[i] * a;
            return res_vec;
        }

        Vec operator/(const T &a) const {
            Vec res_vec;
            for (size_t i = 0; i < Size; ++i)
                res_vec[i] = data_[i] / a;
            return res_vec;
        }
};

template <size_t Size, typename T>
Vec<Size, T> operator*(const T &a, const Vec<Size, T> &v) {
    Vec res_vec;
    for (size_t i = 0; i < Size; ++i)
        res_vec[i] = v[i] * a;
    return res_vec;
}

template <size_t Size, typename T>
std::ostream &operator<<(std::ostream &out, Vec<Size, T> &v) {
    out << "(" << v[0];
    for (size_t i = 1; i < Size; ++i)
        out << ", " << v[i];
    out << ")";
    return out;
}

template <typename T>
Vec<3, T> cross(const Vec<3, T> &v1, const Vec<3, T> &v2) {
    return Vec<3, T>(v1.y * v2.z - v1.z * v2.y,
                     v1.z * v2.x - v1.x * v2.z,
                     v1.x * v2.y - v1.y * v2.x);
}

typedef Vec<2, int> Vec2i;
typedef Vec<3, int> Vec3i;

typedef Vec<2, float> Vec2f;
typedef Vec<3, float> Vec3f;
typedef Vec<4, float> Vec4f;

#endif // __VEC_HH__