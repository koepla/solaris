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

#include <libsolaris/math.h>
#include <math.h>

/// Retrieves the absolute value
f64 math_abs(f64 x) {
    return x < 0 ? -x : x;
}

/// Floors the value
f64 math_floor(f64 x) {
    return floor(x);
}

/// Retrieves the square root of the value
f64 math_sqrt(f64 x) {
    return sqrt(x);
}

/// Converts radians to degrees
f64 math_degrees(f64 radians) {
    return radians * 180.0 / PI;
}

/// Converts degrees to radians
f64 math_radians(f64 degrees) {
    return degrees * PI / 180.0;
}

/// Retrieves the fractional part of the number
f64 math_fraction(f64 x) {
    return x - floor(x);
}

/// Performs modulo operation on the numbers
f64 math_modulo(f64 a, f64 b) {
    return b * math_fraction(a / b);
}

/// Retrieves the sine value of the specified angle
f64 math_sine(f64 angle) {
    return sin(math_radians(angle));
}

/// Retrieves the cosine value of the specified angle
f64 math_cosine(f64 angle) {
    return cos(math_radians(angle));
}

/// Retrieves the tangent value of the specified angle
f64 math_tangent(f64 angle) {
    return tan(math_radians(angle));
}

/// Retrieves the sine angle of the specified fraction
f64 math_arc_sine(f64 x) {
    return math_degrees(asin(x));
}

/// Retrieves the cosine angle of the specified fraction
f64 math_arc_cosine(f64 x) {
    return math_degrees(acos(x));
}

/// Retrieves the tangent angle of the specified fraction
f64 math_arc_tangent(f64 x) {
    return math_degrees(atan(x));
}

/// Retrieves the tangent angle of the specified fraction
f64 math_arc_tangent2(f64 y, f64 x) {
    return math_degrees(atan2(y, x));
}

/// Converts degrees, arc minutes and arc seconds to fractional degrees
f64 math_daa_to_degrees(f64 degrees, f64 arc_minutes, f64 arc_seconds) {
    f64 result = math_abs(degrees) + math_abs(arc_minutes) / 60.0 + math_abs(arc_seconds) / 3600.0;
    return degrees < 0.0 ? -result : result;
}

/// Converts hour, minute and second to fractional degrees
f64 math_hms_to_degrees(f64 hour, f64 minute, f64 second) {
    return 15.0 * (hour + minute / 60.0 + second / 3600.0);
}
