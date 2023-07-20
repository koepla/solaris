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

#ifndef SOLARIS_MATH_H
#define SOLARIS_MATH_H

#include <libsolaris/types.h>

#define PI 3.14159265358979323846
#define PI2 (2.0 * PI)
#define C0 299792458.0
#define AU 149597870700.0
#define ARCS (3600.0 * 180.0 / PI)
#define SECONDS_PER_DAY 86400.0

/// Retrieves the absolute value
/// @param x The value
/// @return Absolute value of x
f64 math_abs(f64 x);

/// Floors the value
/// @param x The value
/// @return Floored value of x
f64 math_floor(f64 x);

/// Retrieves the square root of the value
/// @param x The value
/// @return Square root of x
f64 math_sqrt(f64 x);

/// Converts radians to degrees
/// @param radians The radians
/// @return Radians in degrees
f64 math_degrees(f64 radians);

/// Converts degrees to radians
/// @param degrees The degrees
/// @return Degrees in radians
f64 math_radians(f64 degrees);

/// Retrieves the fractional part of the number
/// @param x The number
/// @return Fraction of the number
f64 math_fraction(f64 x);

/// Performs modulo operation on the numbers
/// @param a Left operand
/// @param b Right operand
/// @return Modulo of operands
f64 math_modulo(f64 a, f64 b);

/// Retrieves the sine value of the specified angle
/// @param angle The angle
/// @return The sine value
f64 math_sine(f64 angle);

/// Retrieves the cosine value of the specified angle
/// @param angle The angle
/// @return The cosine value
f64 math_cosine(f64 angle);

/// Retrieves the tangent value of the specified angle
/// @param angle The angle
/// @return The tangent value
f64 math_tangent(f64 angle);

/// Retrieves the sine angle of the specified fraction
/// @param x The fraction
/// @return The sine angle
f64 math_arc_sine(f64 x);

/// Retrieves the cosine angle of the specified fraction
/// @param x The fraction
/// @return The cosine angle
f64 math_arc_cosine(f64 x);

/// Retrieves the tangent angle of the specified fraction
/// @param x The fraction
/// @return The tangent angle
f64 math_arc_tangent(f64 x);

/// Retrieves the tangent angle of the specified fraction
/// @param y The y distance
/// @param x The x distance
/// @return The tangent angle
///
/// @note This checks for all four areas of the unit circle
f64 math_arc_tangent2(f64 y, f64 x);

/// Converts degrees, arc minutes and arc seconds to fractional degrees
/// @param degrees The amount of degrees
/// @param arc_minutes The amount of arc minutes
/// @param arc_seconds The amount of arc seconds
f64 math_daa_to_degrees(f64 degrees, f64 arc_minutes, f64 arc_seconds);

/// Converts hour, minute and second to fractional degrees
/// @param hour The hour
/// @param minute The minute
/// @param second The second
f64 math_hms_to_degrees(f64 hour, f64 minute, f64 second);

#endif// SOLARIS_MATH_H
