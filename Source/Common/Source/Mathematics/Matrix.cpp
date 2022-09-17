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
    void MatrixMultiply(struct Matrix4x4* dest, const struct Matrix4x4* m1, const struct Matrix4x4* m2)
    {
        dest->_11 = m1->_14 * m2->_41 + m1->_11 * m2->_11 + m1->_12 * m2->_21 + m2->_31 * m1->_13;
        dest->_12 = m2->_42 * m1->_14 + m1->_11 * m2->_12 + m1->_12 * m2->_22 + m2->_32 * m1->_13;
        dest->_13 = m2->_43 * m1->_14 + m2->_13 * m1->_11 + m1->_12 * m2->_23 + m2->_33 * m1->_13;
        dest->_14 = m2->_14 * m1->_11 + m2->_34 * m1->_13 + m1->_12 * m2->_24 + m1->_14 * m2->_44;
        dest->_21 = m1->_21 * m2->_11 + m2->_31 * m1->_23 + m1->_24 * m2->_41 + m1->_22 * m2->_21;
        dest->_22 = m2->_32 * m1->_23 + m2->_42 * m1->_24 + m1->_21 * m2->_12 + m2->_22 * m1->_22;
        dest->_23 = m1->_21 * m2->_13 + m2->_33 * m1->_23 + m2->_43 * m1->_24 + m1->_22 * m2->_23;
        dest->_24 = m1->_22 * m2->_24 + m1->_23 * m2->_34 + m1->_24 * m2->_44 + m1->_21 * m2->_14;
        dest->_31 = m1->_31 * m2->_11 + m2->_31 * m1->_33 + m1->_34 * m2->_41 + m1->_32 * m2->_21;
        dest->_32 = m2->_32 * m1->_33 + m2->_42 * m1->_34 + m1->_31 * m2->_12 + m2->_22 * m1->_32;
        dest->_33 = m1->_31 * m2->_13 + m2->_33 * m1->_33 + m2->_43 * m1->_34 + m1->_32 * m2->_23;
        dest->_34 = m1->_32 * m2->_24 + m1->_33 * m2->_34 + m1->_34 * m2->_44 + m1->_31 * m2->_14;
        dest->_41 = m1->_41 * m2->_11 + m2->_31 * m1->_43 + m1->_44 * m2->_41 + m1->_42 * m2->_21;
        dest->_42 = m2->_32 * m1->_43 + m2->_42 * m1->_44 + m1->_41 * m2->_12 + m2->_22 * m1->_42;
        dest->_43 = m1->_41 * m2->_13 + m2->_33 * m1->_43 + m2->_43 * m1->_44 + m1->_42 * m2->_23;
        dest->_44 = m1->_42 * m2->_24 + m1->_43 * m2->_34 + m1->_44 * m2->_44 + m1->_41 * m2->_14;
    }

    void MatrixInvert(struct Matrix4x4* dest, const struct Matrix4x4* m)
    {
        auto A2323 = m->_33 * m->_44 - m->_34 * m->_43;
        auto A1323 = m->_32 * m->_44 - m->_34 * m->_42;
        auto A1223 = m->_32 * m->_43 - m->_33 * m->_42;
        auto A0323 = m->_31 * m->_44 - m->_34 * m->_41;
        auto A0223 = m->_31 * m->_43 - m->_33 * m->_41;
        auto A0123 = m->_31 * m->_42 - m->_32 * m->_41;
        auto A2313 = m->_23 * m->_44 - m->_24 * m->_43;
        auto A1313 = m->_22 * m->_44 - m->_24 * m->_42;
        auto A1213 = m->_22 * m->_43 - m->_23 * m->_42;
        auto A2312 = m->_23 * m->_34 - m->_24 * m->_33;
        auto A1312 = m->_22 * m->_34 - m->_24 * m->_32;
        auto A1212 = m->_22 * m->_33 - m->_23 * m->_32;
        auto A0313 = m->_21 * m->_44 - m->_24 * m->_41;
        auto A0213 = m->_21 * m->_43 - m->_23 * m->_41;
        auto A0312 = m->_21 * m->_34 - m->_24 * m->_31;
        auto A0212 = m->_21 * m->_33 - m->_23 * m->_31;
        auto A0113 = m->_21 * m->_42 - m->_22 * m->_41;
        auto A0112 = m->_21 * m->_32 - m->_22 * m->_31;

        auto det = m->_11 * (m->_22 * A2323 - m->_23 * A1323 + m->_24 * A1223)
            - m->_12 * (m->_21 * A2323 - m->_23 * A0323 + m->_24 * A0223)
            + m->_13 * (m->_21 * A1323 - m->_22 * A0323 + m->_24 * A0123)
            - m->_14 * (m->_21 * A1223 - m->_22 * A0223 + m->_23 * A0123);
        det = 1.0f / det;

        dest->_11 = det * (m->_22 * A2323 - m->_23 * A1323 + m->_24 * A1223);
        dest->_12 = det * -(m->_12 * A2323 - m->_13 * A1323 + m->_14 * A1223);
        dest->_13 = det * (m->_12 * A2313 - m->_13 * A1313 + m->_14 * A1213);
        dest->_14 = det * -(m->_12 * A2312 - m->_13 * A1312 + m->_14 * A1212);
        dest->_21 = det * -(m->_21 * A2323 - m->_23 * A0323 + m->_24 * A0223);
        dest->_22 = det * (m->_11 * A2323 - m->_13 * A0323 + m->_14 * A0223);
        dest->_23 = det * -(m->_11 * A2313 - m->_13 * A0313 + m->_14 * A0213);
        dest->_24 = det * (m->_11 * A2312 - m->_13 * A0312 + m->_14 * A0212);
        dest->_31 = det * (m->_21 * A1323 - m->_22 * A0323 + m->_24 * A0123);
        dest->_32 = det * -(m->_11 * A1323 - m->_12 * A0323 + m->_14 * A0123);
        dest->_33 = det * (m->_11 * A1313 - m->_12 * A0313 + m->_14 * A0113);
        dest->_34 = det * -(m->_11 * A1312 - m->_12 * A0312 + m->_14 * A0112);
        dest->_41 = det * -(m->_21 * A1223 - m->_22 * A0223 + m->_23 * A0123);
        dest->_42 = det * (m->_11 * A1223 - m->_12 * A0223 + m->_13 * A0123);
        dest->_43 = det * -(m->_11 * A1213 - m->_12 * A0213 + m->_13 * A0113);
        dest->_44 = det * (m->_11 * A1212 - m->_12 * A0212 + m->_13 * A0112);
    }
}