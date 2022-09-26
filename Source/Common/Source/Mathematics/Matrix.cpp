/*
Copyright (c) 2022 AABBWare

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Mathematics/Matrix.h"

namespace Mathematics
{
    void Invert(struct Matrix4x4* dest, const struct Matrix4x4* m)
    {
        auto A2323 = m->m33 * m->m44 - m->m34 * m->m43;
        auto A1323 = m->m32 * m->m44 - m->m34 * m->m42;
        auto A1223 = m->m32 * m->m43 - m->m33 * m->m42;
        auto A0323 = m->m31 * m->m44 - m->m34 * m->m41;
        auto A0223 = m->m31 * m->m43 - m->m33 * m->m41;
        auto A0123 = m->m31 * m->m42 - m->m32 * m->m41;
        auto A2313 = m->m23 * m->m44 - m->m24 * m->m43;
        auto A1313 = m->m22 * m->m44 - m->m24 * m->m42;
        auto A1213 = m->m22 * m->m43 - m->m23 * m->m42;
        auto A2312 = m->m23 * m->m34 - m->m24 * m->m33;
        auto A1312 = m->m22 * m->m34 - m->m24 * m->m32;
        auto A1212 = m->m22 * m->m33 - m->m23 * m->m32;
        auto A0313 = m->m21 * m->m44 - m->m24 * m->m41;
        auto A0213 = m->m21 * m->m43 - m->m23 * m->m41;
        auto A0312 = m->m21 * m->m34 - m->m24 * m->m31;
        auto A0212 = m->m21 * m->m33 - m->m23 * m->m31;
        auto A0113 = m->m21 * m->m42 - m->m22 * m->m41;
        auto A0112 = m->m21 * m->m32 - m->m22 * m->m31;

        auto det = m->m11 * (m->m22 * A2323 - m->m23 * A1323 + m->m24 * A1223)
            - m->m12 * (m->m21 * A2323 - m->m23 * A0323 + m->m24 * A0223)
            + m->m13 * (m->m21 * A1323 - m->m22 * A0323 + m->m24 * A0123)
            - m->m14 * (m->m21 * A1223 - m->m22 * A0223 + m->m23 * A0123);
        det = 1.0f / det;

        dest->m11 = det * (m->m22 * A2323 - m->m23 * A1323 + m->m24 * A1223);
        dest->m12 = det * -(m->m12 * A2323 - m->m13 * A1323 + m->m14 * A1223);
        dest->m13 = det * (m->m12 * A2313 - m->m13 * A1313 + m->m14 * A1213);
        dest->m14 = det * -(m->m12 * A2312 - m->m13 * A1312 + m->m14 * A1212);
        dest->m21 = det * -(m->m21 * A2323 - m->m23 * A0323 + m->m24 * A0223);
        dest->m22 = det * (m->m11 * A2323 - m->m13 * A0323 + m->m14 * A0223);
        dest->m23 = det * -(m->m11 * A2313 - m->m13 * A0313 + m->m14 * A0213);
        dest->m24 = det * (m->m11 * A2312 - m->m13 * A0312 + m->m14 * A0212);
        dest->m31 = det * (m->m21 * A1323 - m->m22 * A0323 + m->m24 * A0123);
        dest->m32 = det * -(m->m11 * A1323 - m->m12 * A0323 + m->m14 * A0123);
        dest->m33 = det * (m->m11 * A1313 - m->m12 * A0313 + m->m14 * A0113);
        dest->m34 = det * -(m->m11 * A1312 - m->m12 * A0312 + m->m14 * A0112);
        dest->m41 = det * -(m->m21 * A1223 - m->m22 * A0223 + m->m23 * A0123);
        dest->m42 = det * (m->m11 * A1223 - m->m12 * A0223 + m->m13 * A0123);
        dest->m43 = det * -(m->m11 * A1213 - m->m12 * A0213 + m->m13 * A0113);
        dest->m44 = det * (m->m11 * A1212 - m->m12 * A0212 + m->m13 * A0112);
    }

    void Multiply(struct Matrix4x4* dest, const struct Matrix4x4* m1, const struct Matrix4x4* m2)
    {
        dest->m11 = m1->m14 * m2->m41 + m1->m11 * m2->m11 + m1->m12 * m2->m21 + m2->m31 * m1->m13;
        dest->m12 = m2->m42 * m1->m14 + m1->m11 * m2->m12 + m1->m12 * m2->m22 + m2->m32 * m1->m13;
        dest->m13 = m2->m43 * m1->m14 + m2->m13 * m1->m11 + m1->m12 * m2->m23 + m2->m33 * m1->m13;
        dest->m14 = m2->m14 * m1->m11 + m2->m34 * m1->m13 + m1->m12 * m2->m24 + m1->m14 * m2->m44;
        dest->m21 = m1->m21 * m2->m11 + m2->m31 * m1->m23 + m1->m24 * m2->m41 + m1->m22 * m2->m21;
        dest->m22 = m2->m32 * m1->m23 + m2->m42 * m1->m24 + m1->m21 * m2->m12 + m2->m22 * m1->m22;
        dest->m23 = m1->m21 * m2->m13 + m2->m33 * m1->m23 + m2->m43 * m1->m24 + m1->m22 * m2->m23;
        dest->m24 = m1->m22 * m2->m24 + m1->m23 * m2->m34 + m1->m24 * m2->m44 + m1->m21 * m2->m14;
        dest->m31 = m1->m31 * m2->m11 + m2->m31 * m1->m33 + m1->m34 * m2->m41 + m1->m32 * m2->m21;
        dest->m32 = m2->m32 * m1->m33 + m2->m42 * m1->m34 + m1->m31 * m2->m12 + m2->m22 * m1->m32;
        dest->m33 = m1->m31 * m2->m13 + m2->m33 * m1->m33 + m2->m43 * m1->m34 + m1->m32 * m2->m23;
        dest->m34 = m1->m32 * m2->m24 + m1->m33 * m2->m34 + m1->m34 * m2->m44 + m1->m31 * m2->m14;
        dest->m41 = m1->m41 * m2->m11 + m2->m31 * m1->m43 + m1->m44 * m2->m41 + m1->m42 * m2->m21;
        dest->m42 = m2->m32 * m1->m43 + m2->m42 * m1->m44 + m1->m41 * m2->m12 + m2->m22 * m1->m42;
        dest->m43 = m1->m41 * m2->m13 + m2->m33 * m1->m43 + m2->m43 * m1->m44 + m1->m42 * m2->m23;
        dest->m44 = m1->m42 * m2->m24 + m1->m43 * m2->m34 + m1->m44 * m2->m44 + m1->m41 * m2->m14;
    }
}