#ifndef __GEOMETRY_HH__
#define __GEOMETRY_HH__

#include <cmath>

/////////////////////////////////////////

template <class T> struct Vec2 {
    union {
        struct { T x, y; };
        struct { T u, v; };
        T raw[2];
    };
    
    Vec2()
        : u(0)
        , v(0) { }
    
    Vec2(T _x, T _y)
        : u(_x)
        , v(_y) { }
    
    template <class> friend std::ostream &operator<<(std::ostream &s, Vec2<T> &v);
    
    inline Vec2<T> operator+(const Vec2<T> &v) const { return Vec2<T>(x + v.x, y + v.y); }
    inline Vec2<T> operator-(const Vec2<T> &v) const { return Vec2<T>(x - v.x, y - v.y); }
    
    inline Vec2<T> operator*(float f) const { return Vec2<T>(x * f, y * f); }
};

/////////////////////////////////////////

template <class T> struct Vec3 {
    union {
        struct { T x, y, z; };
        struct { T ivert, iuv, inorm; };
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

    template <class> friend std::ostream &operator<<(std::ostream &s, Vec3<T> &v);
    
    inline Vec3<T> operator+(const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
    inline Vec3<T> operator-(const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
    
    inline Vec3<T> operator*(float f) const { return Vec3<T>(x * f, y * f, z * f); }

    inline T operator*(const Vec3<T> &v) const {
        return x * v.x + y * v.y + z * v.z; // dot product
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

template <class T> std::ostream &operator<<(std::ostream &s, Vec2<T> &v) {
    s << "(" << v.x << ", " << v.y << ")";
    return s;
}

template <class T> std::ostream &operator<<(std::ostream &s, Vec3<T> &v) {
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