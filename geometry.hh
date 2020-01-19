#ifndef __GEOMETRY_HH__
#define __GEOMETRY_HH__

#include <cassert>
#include <cmath>

/////////////////////////////////////////

template <class T> struct Vec2 {
    union {
        struct { T x, y; };
        T raw[2];
    };
    
    Vec2()
        : x(0)
        , y(0) { }
    
    Vec2(T _x, T _y)
        : x(_x)
        , y(_y) { }
    
    template <typename OtherT> explicit Vec2(const Vec2<OtherT> &other) {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
    }
    
    template <class> friend std::ostream &operator<<(std::ostream &out, Vec2<T> &v);

    T &operator[](const unsigned int i) { assert(i < 2); return raw[i]; }
    const T &operator[](const unsigned int i) const { assert(i < 2); return raw[i]; }
    
    inline Vec2<T> operator+(const Vec2<T> &v) const { return Vec2<T>(x + v.x, y + v.y); }
    inline Vec2<T> operator-(const Vec2<T> &v) const { return Vec2<T>(x - v.x, y - v.y); }
    
    inline Vec2<T> operator*(float f) const { return Vec2<T>(x * f, y * f); }
    
    inline Vec2<T> operator*(const Vec2<T> &v) const {
        return Vec2(x * v.x, y * v.y); // elementwise multiplication
    }
};

/////////////////////////////////////////

template <class T> struct Vec3 {
    union {
        struct { T x, y, z; };
        struct { T ivertex, iuv, inormal; }; // { v, vt, vn }
        T raw[3];
    };

    Vec3()
        : x(0)
        , y(0)
        , z(0) { }
    
    Vec3(T _x, T _y, T _z)
        : x(_x)
        , y(_y)
        , z(_z) { }
    
    template <typename OtherT> explicit Vec3(const Vec3<OtherT> &other) {
        x = static_cast<T>(other.x);
        y = static_cast<T>(other.y);
        z = static_cast<T>(other.z);
    }

    template <class> friend std::ostream &operator<<(std::ostream &out, Vec3<T> &v);
    
    T &operator[](const unsigned int i) { assert(i < 3); return raw[i]; }
    const T &operator[](const unsigned int i) const { assert(i < 3); return raw[i]; }
    
    inline Vec3<T> operator+(const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
    inline Vec3<T> operator-(const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
    
    inline Vec3<T> operator*(float f) const { return Vec3<T>(x * f, y * f, z * f); }

    inline Vec3<T> operator*(const Vec3<T> &v) const {
        return Vec3(x * v.x, y * v.y, z * v.z); // elementwise multiplication
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float length_squared() const {
        return x * x + y * y + z * z;
    }

    Vec3<T> &normalize(T l = 1) {
        *this = (*this) * (l / length());
        return *this;
    }

    inline const Vec2<T> &xy() const {
        return *(Vec2<T> *) this;
    }
};

/////////////////////////////////////////

template <class T> std::ostream &operator<<(std::ostream &out, Vec2<T> &v) {
    s << "(" << v.x << ", " << v.y << ")";
    return s;
}

template <class T> std::ostream &operator<<(std::ostream &out, Vec3<T> &v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return s;
}

template <class T> inline Vec2<T> operator*(float f, const Vec2<T> &v) {
    return Vec2<T>(v.u * f, v.u * f);
}

template <class T> inline Vec3<T> operator*(float f, const Vec3<T> &v) {
    return Vec3<T>(v.x * f, v.y * f, v.z * f);
}

template <class T> inline T dot(const Vec3<T> &v1, const Vec3<T> &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; // v1 * v2
}

template <class T> inline Vec3<T> cross(const Vec3<T> &v1, const Vec3<T> &v2) {
    return Vec3<T>(v1.y * v2.z - v1.z * v2.y, 
                   v1.z * v2.x - v1.x * v2.z, 
                   v1.x * v2.y - v1.y * v2.x);
}

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

#endif //__GEOMETRY_HH__