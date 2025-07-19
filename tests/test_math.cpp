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
#include <solaris/math.h>

TEST(MathTest, Abs) {
    EXPECT_DOUBLE_EQ(math_abs(5.0), 5.0);
    EXPECT_DOUBLE_EQ(math_abs(-5.0), 5.0);
    EXPECT_DOUBLE_EQ(math_abs(0.0), 0.0);
}

TEST(MathTest, Floor) {
    EXPECT_DOUBLE_EQ(math_floor(3.7), 3.0);
    EXPECT_DOUBLE_EQ(math_floor(-2.3), -3.0);
}

TEST(MathTest, Sqrt) {
    EXPECT_DOUBLE_EQ(math_sqrt(9.0), 3.0);
    EXPECT_DOUBLE_EQ(math_sqrt(0.0), 0.0);
}

TEST(MathTest, Degrees) {
    EXPECT_NEAR(math_degrees(PI), 180.0, 1e-9);
    EXPECT_NEAR(math_degrees(PI2), 360.0, 1e-9);
}

TEST(MathTest, Radians) {
    EXPECT_NEAR(math_radians(180.0), PI, 1e-9);
    EXPECT_NEAR(math_radians(360.0), PI2, 1e-9);
}

TEST(MathTest, Fraction) {
    EXPECT_NEAR(math_fraction(4.75), 0.75, 1e-9);
    EXPECT_NEAR(math_fraction(-2.25), 0.75, 1e-9);// depending on implementation
}

TEST(MathTest, Modulo) {
    EXPECT_DOUBLE_EQ(math_modulo(10.5, 3.0), fmod(10.5, 3.0));
    EXPECT_DOUBLE_EQ(math_modulo(-10.5, 3.0), fmod(-10.5, 3.0));
}

TEST(MathTest, RadiansAndDegrees) {
    // 180° == π, 360° == 2π
    EXPECT_NEAR(math_radians(180.0), PI, 1e-9);
    EXPECT_NEAR(math_radians(360.0), PI2, 1e-9);

    EXPECT_NEAR(math_degrees(PI), 180.0, 1e-9);
    EXPECT_NEAR(math_degrees(PI2), 360.0, 1e-9);
}

TEST(MathTest, Trigonometric) {
    // These expect degree input
    EXPECT_NEAR(math_sine(90.0), 1.0, 1e-9);
    EXPECT_NEAR(math_cosine(0.0), 1.0, 1e-9);
    EXPECT_NEAR(math_tangent(45.0), 1.0, 1e-9);
}

TEST(MathTest, ArcTrigonometric) {
    // Arc-functions return degree output
    EXPECT_NEAR(math_arc_sine(1.0), 90.0, 1e-9);
    EXPECT_NEAR(math_arc_cosine(1.0), 0.0, 1e-9);
    EXPECT_NEAR(math_arc_tangent(1.0), 45.0, 1e-9);
}

TEST(MathTest, ArcTangent2) {
    EXPECT_NEAR(math_arc_tangent2(1.0, 1.0), 45.0, 1e-9);
    EXPECT_NEAR(math_arc_tangent2(-1.0, -1.0), -3.0 * 45.0, 1e-9);
}

TEST(MathTest, DAAtoDegrees) {
    EXPECT_DOUBLE_EQ(math_daa_to_degrees(10, 30, 0), 10.5);
    EXPECT_DOUBLE_EQ(math_daa_to_degrees(0, 0, 45), 0.0125);
}

TEST(MathTest, HMStoDegrees) {
    EXPECT_DOUBLE_EQ(math_hms_to_degrees(1, 0, 0), 15.0);
    EXPECT_DOUBLE_EQ(math_hms_to_degrees(0, 30, 0), 7.5);
    EXPECT_DOUBLE_EQ(math_hms_to_degrees(0, 0, 30), 0.125);
}