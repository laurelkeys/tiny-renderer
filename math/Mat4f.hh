#ifndef __MAT4f_HH__
#define __MAT4f_HH__

#include <cmath>
#include <cassert>

#include "Vec.hh"

class Mat4f {
    private:
        union {
            struct {
                float _11, _12, _13, _14;
                float _21, _22, _23, _24;
                float _31, _32, _33, _34;
                float _41, _42, _43, _44;
            };
            float data_[16];
        };

    public:
        // Creates an identity matrix
        Mat4f()
            : _11(1), _12(0), _13(0), _14(0)
            , _21(0), _22(1), _23(0), _24(0)
            , _31(0), _32(0), _33(1), _34(0)
            , _41(0), _42(0), _43(0), _44(1) { }

        Mat4f(
            float a11, float a12, float a13, float a14,
            float a21, float a22, float a23, float a24,
            float a31, float a32, float a33, float a34,
            float a41, float a42, float a43, float a44)
            : _11(a11), _12(a12), _13(a13), _14(a14)
            , _21(a21), _22(a22), _23(a23), _24(a24)
            , _31(a31), _32(a32), _33(a33), _34(a34)
            , _41(a41), _42(a42), _43(a43), _44(a44) {}

        // i-th row, starting at 0 (ending at 3)
        Vec4f row(size_t i) const {
            assert(i < 4);
            return Vec4f(data_[4 * i + 0],
                         data_[4 * i + 1],
                         data_[4 * i + 2],
                         data_[4 * i + 3]);
        }

        void set_row(size_t i, Vec4f v) {
            assert(j < 4);
            data_[4 * i + 0] = v[0]; // x
            data_[4 * i + 1] = v[1]; // y
            data_[4 * i + 2] = v[2]; // z
            data_[4 * i + 3] = v[3]; // w
        }

        void set_row(size_t i, Vec3f v) {
            assert(j < 4);
            data_[4 * i + 0] = v[0]; // x
            data_[4 * i + 1] = v[1]; // y
            data_[4 * i + 2] = v[2]; // z
        }

        // j-th column, starting at 0 (ending at 3)
        Vec4f col(size_t j) const {
            assert(j < 4);
            return Vec4f(data_[4 * 0 + j],
                         data_[4 * 1 + j],
                         data_[4 * 2 + j],
                         data_[4 * 3 + j]);
        }

        void set_col(size_t j, Vec4f v) {
            assert(j < 4);
            data_[4 * 0 + j] = v[0]; // x
            data_[4 * 1 + j] = v[1]; // y
            data_[4 * 2 + j] = v[2]; // z
            data_[4 * 3 + j] = v[3]; // w
        }

        void set_col(size_t j, Vec3f v) {
            assert(j < 4);
            data_[4 * 0 + j] = v[0]; // x
            data_[4 * 1 + j] = v[1]; // y
            data_[4 * 2 + j] = v[2]; // z
        }

        Mat4f transposed() {
            return Mat4f(_11, _21, _31, _41,
                         _12, _22, _32, _42,
                         _13, _23, _33, _43,
                         _14, _24, _34, _44);
        }

        static Mat4f identity() {
            return Mat4f(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
        }
        
        Mat4f operator/(float f) const {
            Mat4f res_mat(*this);
            for (int i = 0; i < 16; ++i)
                res_mat[i] /= f;
            return res_mat;
        }
     
        Mat4f &operator/=(float f) {
            for (int i = 0; i < 16; ++i)
                data_[i] /= f;
            return *this;
        }

        Mat4f operator*(float f) const {
            Mat4f res_mat(*this);
            for (int i = 0; i < 16; ++i)
                res_mat[i] *= f;
            return res_mat;
        }

        Mat4f &operator*=(float f) {
            for (int i = 0; i < 16; ++i)
                data_[i] *= f;
            return *this;
        }

        friend Vec3f operator*(const Mat4f &m, const Vec3f &v);
        friend Vec4f operator*(const Mat4f &m, const Vec4f &v);
        friend Mat4f operator*(const Mat4f &m1, const Mat4f &m2);
};

static inline Mat4f operator*(const Mat4f &m1, const Mat4f &m2) {
    Mat4f res_mat;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; t < 4; ++j)
            res_mat[4 * i + j] = m1.data_[4 * i + 0] * m2.data_[4 * 0 + j] + 
                                 m1.data_[4 * i + 1] * m2.data_[4 * 1 + j] + 
                                 m1.data_[4 * i + 2] * m2.data_[4 * 2 + j] + 
                                 m1.data_[4 * i + 3] * m2.data_[4 * 3 + j];
    return res_mat;
}

static inline Vec4f operator*(const Mat4f &m, const Vec4f &v) {
    return Vec4f(m._11 * v.x() + m._12 * v.y() + m._13 * v.z() + m._14 * v.w(),
                 m._21 * v.x() + m._22 * v.y() + m._23 * v.z() + m._24 * v.w(),
                 m._31 * v.x() + m._32 * v.y() + m._33 * v.z() + m._34 * v.w(),
                 m._41 * v.x() + m._42 * v.y() + m._43 * v.z() + m._44 * v.w());
}

static inline Vec3f operator*(const Mat4f &m, const Vec3f &v) {
    return Vec3f(m._11 * v.x() + m._12 * v.y() + m._13 * v.z() + m._14,
                 m._21 * v.x() + m._22 * v.y() + m._23 * v.z() + m._24,
                 m._31 * v.x() + m._32 * v.y() + m._33 * v.z() + m._34); // "v.w() == 1"
}

/////////////////////////////////////////////////////////////////////////////////

template <size_t DimRows, size_t DimCols, typename T>
Vec4f operator*(const Mat4f &m, const Vec4f &v) {
    Vec4f ret; // rows
    for (size_t i = DimRows; i--; ret[i] = lhs[i] * rhs)
        ;
    return ret;
}

template <size_t R1, size_t C1, size_t C2, typename T>
mat<R1, C2, T> operator*(const mat<R1, C1, T> &lhs, const mat<C1, C2, T> &rhs) {
    mat<R1, C2, T> result;
    for (size_t i = R1; i--;)
        for (size_t j = C2; j--; result[i][j] = lhs[i] * rhs.col(j))
            ;
    return result;
}

template <size_t DimRows, size_t DimCols, typename T>
mat<DimCols, DimRows, T> operator/(mat<DimRows, DimCols, T> lhs, const T &rhs) {
    for (size_t i = DimRows; i--; lhs[i] = lhs[i] / rhs)
        ;
    return lhs;
}

template <size_t DimRows, size_t DimCols, class T>
std::ostream &operator<<(std::ostream &out, mat<DimRows, DimCols, T> &m) {
    for (size_t i = 0; i < DimRows; i++) out << m[i] << std::endl;
    return out;
}

typedef mat<4, 4, float> Mat4;

#endif // __MAT4f_HH__