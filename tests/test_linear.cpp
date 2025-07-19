//
// MIT License
//
// Copyright (c) 2023 Elias Engelbert Plank
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <gtest/gtest.h>
#include <solaris/linear.h>

#define NEAR_EQUAL(a, b) EXPECT_NEAR((a), (b), 1e-9)

TEST(LinearTest, Vector3Length) {
    Vector3 constexpr v = { 3.0, 4.0, 12.0 };
    NEAR_EQUAL(vector3_length(&v), 13.0);
}

TEST(LinearTest, Vector3Add) {
    Vector3 constexpr a = { 1.0, 2.0, 3.0 };
    Vector3 constexpr b = { 4.0, -1.0, 0.5 };
    auto const [x, y, z] = vector3_add(&a, &b);
    NEAR_EQUAL(x, 5.0);
    NEAR_EQUAL(y, 1.0);
    NEAR_EQUAL(z, 3.5);
}

TEST(LinearTest, Vector3Sub) {
    Vector3 constexpr a = { 4.0, 5.0, 6.0 };
    Vector3 constexpr b = { 1.0, 2.0, 3.0 };
    auto const [x, y, z] = vector3_sub(&a, &b);
    NEAR_EQUAL(x, 3.0);
    NEAR_EQUAL(y, 3.0);
    NEAR_EQUAL(z, 3.0);
}

TEST(LinearTest, Matrix3x3Diagonal) {
    Matrix3x3 const m = matrix3x3_diagonal(7.0);
    for (usize i = 0; i < 3; ++i) {
        for (usize j = 0; j < 3; ++j) {
            if (i == j) {
                NEAR_EQUAL(m.elements[i][j], 7.0);
            } else {
                NEAR_EQUAL(m.elements[i][j], 0.0);
            }
        }
    }
}

TEST(LinearTest, Matrix3x3Transpose) {
    Matrix3x3 constexpr m = { { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } } };
    Matrix3x3 const t = matrix3x3_transpose(&m);
    EXPECT_EQ(t.elements[0][1], 4);
    EXPECT_EQ(t.elements[1][0], 2);
    EXPECT_EQ(t.elements[2][0], 3);
    EXPECT_EQ(t.elements[0][2], 7);
}

TEST(LinearTest, Matrix3x3MulIdentity) {
    Matrix3x3 constexpr test = { { { 1, 2, 3 }, { 0, 1, 4 }, { 5, 6, 0 } } };
    Matrix3x3 const id = matrix3x3_diagonal(1.0);
    Matrix3x3 const result = matrix3x3_mul(&id, &test);

    for (usize i = 0; i < 3; ++i) {
        for (usize j = 0; j < 3; ++j) {
            NEAR_EQUAL(result.elements[i][j], test.elements[i][j]);
        }
    }
}

TEST(LinearTest, Matrix3x3MulVector3) {
    Vector3 constexpr v = { 3.0, 4.0, 5.0 };
    Matrix3x3 const id = matrix3x3_diagonal(1.0);
    auto const [x, y, z] = matrix3x3_mul_vector3(&id, &v);
    NEAR_EQUAL(x, v.x);
    NEAR_EQUAL(y, v.y);
    NEAR_EQUAL(z, v.z);
}

TEST(LinearTest, Matrix3x3RotationZ90) {
    Vector3 constexpr v = { 1.0, 0.0, 0.0 };
    Matrix3x3 const rot = matrix3x3_rotation(ROTATION_AXIS_Z, 90.0);
    auto const [x, y, z] = matrix3x3_mul_vector3(&rot, &v);
    NEAR_EQUAL(x, 0.0);
    NEAR_EQUAL(y, 1.0);
    NEAR_EQUAL(z, 0.0);
}

TEST(LinearTest, Matrix3x3MulChain) {
    Matrix3x3 const a = matrix3x3_rotation(ROTATION_AXIS_Z, 90.0);
    Matrix3x3 const b = matrix3x3_rotation(ROTATION_AXIS_Z, -90.0);
    Matrix3x3 const chain[] = { a, b };
    Matrix3x3 const result = matrix3x3_mul_chain(chain, 2);
    Matrix3x3 const id = matrix3x3_diagonal(1.0);

    for (usize i = 0; i < 3; ++i) {
        for (usize j = 0; j < 3; ++j) {
            NEAR_EQUAL(result.elements[i][j], id.elements[i][j]);
        }
    }
}

TEST(LinearTest, Vector3ToEquatorialAndBack) {
    Equatorial constexpr e = { 45.0, 30.0, 2.0 };
    Vector3 const v = vector3_from_equatorial(&e);
    Equatorial const back = equatorial_from_vector3(&v);
    NEAR_EQUAL(e.right_ascension, back.right_ascension);
    NEAR_EQUAL(e.declination, back.declination);
    NEAR_EQUAL(e.distance, back.distance);
}

TEST(LinearTest, LocalEquatorialToHorizontal) {
    Horizontal const h = local_equatorial_to_horizontal(45.0, 0.0, 45.0);
    NEAR_EQUAL(h.azimuth, 180.0);
    NEAR_EQUAL(h.altitude, 90.0);
}

TEST(LinearTest, EclipticDriftSimple) {
    f64 const drift = ecliptic_drift(0.0);
    NEAR_EQUAL(drift, 23.43929111);
}

TEST(LinearTest, ReferencePlaneIdentity) {
    Matrix3x3 const m = matrix3x3_reference_plane(REFERENCE_PLANE_EQUATORIAL, REFERENCE_PLANE_EQUATORIAL, 0.0);
    Matrix3x3 const id = matrix3x3_diagonal(1.0);
    for (usize i = 0; i < 3; ++i) {
        for (usize j = 0; j < 3; ++j) {
            NEAR_EQUAL(m.elements[i][j], id.elements[i][j]);
        }
    }
}
