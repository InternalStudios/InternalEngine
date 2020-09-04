#pragma once

#include <cstdint>

namespace Internal
{

    template<uint32_t t, typename T>
    struct Vector {};

    template<typename T>
    struct Vector<1, T>
    {
        union
        {
            struct {T x;};
            struct {T r;};
        };
    };

    template<typename T>
    struct Vector<2, T>
    {
        union
        {
            struct {T x, y;};
            struct {T r, g;};
        };
    };

    template<typename T>
    struct Vector<3, T>
    {
        union
        {
            struct {T x, y, z;};
            struct {T r, g, b;};
        };
    };

    template<typename T>
    struct Vector<4, T>
    {
        union
        {
            struct {T x, y, z, w;};
            struct {T r, g, b, a;};
        };
    };

    typedef Vector<1, float> Vector1f;
    typedef Vector<2, float> Vector2f;
    typedef Vector<3, float> Vector3f;
    typedef Vector<4, float> Vector4f;

    typedef Vector<1, double> Vector1d;
    typedef Vector<2, double> Vector2d;
    typedef Vector<3, double> Vector3d;
    typedef Vector<4, double> Vector4d;

    typedef Vector<1, uint32_t> Vector1ui;
    typedef Vector<2, uint32_t> Vector2ui;
    typedef Vector<3, uint32_t> Vector3ui;
    typedef Vector<4, uint32_t> Vector4ui;

    typedef Vector<1, int32_t> Vector1i;
    typedef Vector<2, int32_t> Vector2i;
    typedef Vector<3, int32_t> Vector3i;
    typedef Vector<4, int32_t> Vector4i;

    template<uint32_t x, uint32_t y, typename T>
    struct Matrix {};

    template<typename T>
    struct Matrix<1, 1, T>
    {
        Matrix<1, 1, T>(T mx1) : x1(mx1) {}
        T x1;

        Matrix<1, 1, T> operator*(Matrix<1, 1, T> m)
        {
            return Matrix<1, 1, T>(m.x1 * x1);
        }
    };

    template<typename T>
    struct Matrix<1, 2, T>
    {
        Matrix<1, 2, T>(T mx1, T mx2) : x1(mx1), x2(mx2) {}

        T x1, x2;

        Matrix<1, 2, T> operator*(Matrix<1, 2, T> m)
        {
            //return m.x1 * x1;
        }
    };

    template<typename T>
    struct Matrix<1, 3, T>
    {
        Matrix<1, 3, T>(T mx1, T mx2, T mx3) : x1(mx1), x2(mx2), x3(mx3) {}

        T x1, x2, x3;

        Matrix<1, 3, T> operator*(Matrix<1, 3, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<1, 4, T>
    {
        Matrix<1, 4, T>(T mx1, T mx2, T mx3, T mx4) : x1(mx1), x2(mx2), x3(mx3), x4(mx4) {}

        T x1, x2, x3, x4;

        Matrix<1, 4, T> operator*(Matrix<1, 4, T> m)
        {
            //return m.x1 * x1;
        }
    };

    template<typename T>
    struct Matrix<2, 1, T>
    {
        Matrix<2, 1, T>(T mx1, T my1) : x1(mx1), y1(my1)
        {

        }

        T x1,
          y1;

        Matrix<2, 1, T> operator*(Matrix<2, 1, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<2, 2, T>
    {
        Matrix<2, 2, T>(T mx1, T mx2, T my1, T my2) : x1(mx1), x2(mx2), y1(my1), y2(my2)
        {

        }

        T x1, x2,
          y1, y2;

        Matrix<2, 2, T> operator*(Matrix<2, 2, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<2, 3, T>
    {
        Matrix<2, 3, T>(T mx1, T mx2, T mx3, T my1, T my2, T my3) : x1(mx1), x2(mx2), x3(mx3), y1(my1), y2(my2), y3(my3)
        {

        }

        T x1, x2, x3,
          y1, y2, y3;

        Matrix<2, 3, T> operator*(Matrix<2, 3, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<2, 4, T>
    {
        Matrix<2, 4, T>(T mx1, T mx2, T mx3, T mx4, T my1, T my2, T my3, T my4) : x1(mx1), x2(mx2), x3(mx3), x4(mx4), y1(my1), y2(my2), y3(my3), y4(my4) {}

        T x1, x2, x3, x4,
          y1, y2, y3, y4;

        Matrix<2, 4, T> operator*(Matrix<2, 4, T> m)
        {
            //return m.x1 * x1;
        }
    };

    template<typename T>
    struct Matrix<3, 1, T>
    {
        Matrix<3, 1, T>(T mx1, T my1, T mz1) : x1(mx1), y1(my1), z1(mz1) {}

        T x1,
          y1,
          z1;

        Matrix<3, 1, T> operator*(Matrix<3, 1, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<3, 2, T>
    {
        Matrix<3, 2, T>(T mx1, T mx2, T my1, T my2, T mz1, T mz2) : x1(mx1), x2(mx2), y1(my1), y2(my2), z1(mz1), z2(mz2) {}

        T x1, x2,
          y1, y2,
          z1, z2;

        Matrix<3, 2, T> operator*(Matrix<3, 2, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<3, 3, T>
    {
        Matrix<3, 3, T>(T mx1, T mx2, T mx3, T my1, T my2, T my3, T mz1, T mz2, T mz3) : x1(mx1), x2(mx2), x3(mx3), y1(my1), y2(my2), y3(my3), z1(mz1), z2(mz2), z3(mz3) {}

        T x1, x2, x3,
          y1, y2, y3,
          z1, z2, z3;

        Matrix<3, 3, T> operator*(Matrix<3, 3, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<3, 4, T>
    {
        Matrix<3, 4, T>(T mx1, T mx2, T mx3, T mx4, T my1, T my2, T my3, T my4, T mz1, T mz2, T mz3, T mz4) : x1(mx1), x2(mx2), x3(mx3), x4(mx4), y1(my1), y2(my2), y3(my3), y4(my4), z1(mz1), z2(mz2), z3(mz3), z4(mz4) {}

        T x1, x2, x3, x4,
          y1, y2, y3, y4,
          z1, z2, z3, z4;

        Matrix<3, 4, T> operator*(Matrix<3, 4, T> m)
        {
            //return m.x1 * x1;
        }
    };

    template<typename T>
    struct Matrix<4, 1, T>
    {
        Matrix<4, 1, T>(T mx1, T my1, T mz1, T mw1) : x1(mx1), y1(my1), z1(mz1), w1(mw1) {}

        T x1,
          y1,
          z1,
          w1;

        Matrix<4, 1, T> operator*(Matrix<4, 1, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<4, 2, T>
    {
        Matrix<4, 2, T>(T mx1, T mx2, T my1, T my2, T mz1, T mz2, T mw1, T mw2) : x1(mx1), x2(mx2), y1(my1), y2(my2), z1(mz1), z2(mz2), w1(mw1), w2(mw2) {}

        T x1, x2,
          y1, y2,
          z1, z2,
          w1, w2;

        Matrix<4, 2, T> operator*(Matrix<4, 2, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<4, 3, T>
    {
        Matrix<4, 3, T>(T mx1, T mx2, T mx3, T my1, T my2, T my3, T mz1, T mz2, T mz3, T mw1, T mw2, T mw3) : x1(mx1), x2(mx2), x3(mx3), y1(my1), y2(my2), y3(my3), z1(mz1), z2(mz1), z3(mz3), w1(mw1), w2(mw2), w3(mw3) {}

        T x1, x2, x3,
          y1, y2, y3,
          z1, z2, z3,
          w1, w2, w3;

        Matrix<4, 3, T> operator*(Matrix<4, 3, T> m)
        {
            //return m.x1 * x1;
        }
    };
    template<typename T>
    struct Matrix<4, 4, T>
    {
        Matrix<4, 4, T>(T mx1, T mx2, T mx3, T mx4, T my1, T my2, T my3, T my4, T mz1, T mz2, T mz3, T mz4, T mw1, T mw2, T mw3, T mw4) : x1(mx1), x2(mx2), x3(mx3), x4(mx4), y1(my1), y2(my2), y3(my3), y4(my4), z1(mz1), z2(mz1), z3(mz3), z4(mz4), w1(mw1), w2(mw2), w3(mw3), w4(mw4) {}

        T x1, x2, x3, x4,
          y1, y2, y3, y4,
          z1, z2, z3, z4,
          w1, w2, w3, w4;

        Matrix<4, 4, T> operator*(Matrix<4, 4, T> m)
        {
            //return m.x1 * x1;
        }
    };

    typedef Matrix<1, 1, float> Matrix1x1f;
    typedef Matrix<1, 2, float> Matrix1x2f;
    typedef Matrix<1, 3, float> Matrix1x3f;
    typedef Matrix<1, 4, float> Matrix1x4f;

    typedef Matrix<2, 1, float> Matrix2x1f;
    typedef Matrix<2, 2, float> Matrix2x2f;
    typedef Matrix<2, 3, float> Matrix2x3f;
    typedef Matrix<2, 4, float> Matrix2x4f;

    typedef Matrix<3, 1, float> Matrix3x1f;
    typedef Matrix<3, 2, float> Matrix3x2f;
    typedef Matrix<3, 3, float> Matrix3x3f;
    typedef Matrix<3, 4, float> Matrix3x4f;

    typedef Matrix<4, 1, float> Matrix4x1f;
    typedef Matrix<4, 2, float> Matrix4x2f;
    typedef Matrix<4, 3, float> Matrix4x3f;
    typedef Matrix<4, 4, float> Matrix4x4f;

    typedef Matrix<1, 1, double> Matrix1x1d;
    typedef Matrix<1, 2, double> Matrix1x2d;
    typedef Matrix<1, 3, double> Matrix1x3d;
    typedef Matrix<1, 4, double> Matrix1x4d;

    typedef Matrix<2, 1, double> Matrix2x1d;
    typedef Matrix<2, 2, double> Matrix2x2d;
    typedef Matrix<2, 3, double> Matrix2x3d;
    typedef Matrix<2, 4, double> Matrix2x4d;

    typedef Matrix<3, 1, double> Matrix3x1d;
    typedef Matrix<3, 2, double> Matrix3x2d;
    typedef Matrix<3, 3, double> Matrix3x3d;
    typedef Matrix<3, 4, double> Matrix3x4d;

    typedef Matrix<4, 1, double> Matrix4x1d;
    typedef Matrix<4, 2, double> Matrix4x2d;
    typedef Matrix<4, 3, double> Matrix4x3d;
    typedef Matrix<4, 4, double> Matrix4x4d;

    typedef Matrix<1, 1, uint32_t> Matrix1x1ui;
    typedef Matrix<1, 2, uint32_t> Matrix1x2ui;
    typedef Matrix<1, 3, uint32_t> Matrix1x3ui;
    typedef Matrix<1, 4, uint32_t> Matrix1x4ui;

    typedef Matrix<2, 1, uint32_t> Matrix2x1ui;
    typedef Matrix<2, 2, uint32_t> Matrix2x2ui;
    typedef Matrix<2, 3, uint32_t> Matrix2x3ui;
    typedef Matrix<2, 4, uint32_t> Matrix2x4ui;

    typedef Matrix<3, 1, uint32_t> Matrix3x1ui;
    typedef Matrix<3, 2, uint32_t> Matrix3x2ui;
    typedef Matrix<3, 3, uint32_t> Matrix3x3ui;
    typedef Matrix<3, 4, uint32_t> Matrix3x4ui;

    typedef Matrix<4, 1, uint32_t> Matrix4x1ui;
    typedef Matrix<4, 2, uint32_t> Matrix4x2ui;
    typedef Matrix<4, 3, uint32_t> Matrix4x3ui;
    typedef Matrix<4, 4, uint32_t> Matrix4x4ui;

    typedef Matrix<1, 1, int32_t> Matrix1x1i;
    typedef Matrix<1, 2, int32_t> Matrix1x2i;
    typedef Matrix<1, 3, int32_t> Matrix1x3i;
    typedef Matrix<1, 4, int32_t> Matrix1x4i;

    typedef Matrix<2, 1, int32_t> Matrix2x1i;
    typedef Matrix<2, 2, int32_t> Matrix2x2i;
    typedef Matrix<2, 3, int32_t> Matrix2x3i;
    typedef Matrix<2, 4, int32_t> Matrix2x4i;

    typedef Matrix<3, 1, int32_t> Matrix3x1i;
    typedef Matrix<3, 2, int32_t> Matrix3x2i;
    typedef Matrix<3, 3, int32_t> Matrix3x3i;
    typedef Matrix<3, 4, int32_t> Matrix3x4i;

    typedef Matrix<4, 1, int32_t> Matrix4x1i;
    typedef Matrix<4, 2, int32_t> Matrix4x2i;
    typedef Matrix<4, 3, int32_t> Matrix4x3i;
    typedef Matrix<4, 4, int32_t> Matrix4x4i;
}
