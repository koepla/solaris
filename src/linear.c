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

#include <solaris/linear.h>
#include <solaris/math.h>

/// Retrieves the length of the vector
f64 vector3_length(Vector3 const *const vector) {
    return math_sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}

/// Adds the two vector operands
Vector3 vector3_add(Vector3 const *const left, Vector3 const *const right) {
    return (Vector3) { .x = left->x + right->x, .y = left->y + right->y, .z = left->z + right->z };
}

/// Subtracts the two vector operands
Vector3 vector3_sub(Vector3 const *const left, Vector3 const *const right) {
    return (Vector3) { .x = left->x - right->x, .y = left->y - right->y, .z = left->z - right->z };
}

/// Creates a diagonal matrix
Matrix3x3 matrix3x3_diagonal(f64 const diagonal) {
    Matrix3x3 result;
    result.elements[0][0] = diagonal;
    result.elements[0][1] = 0.0f;
    result.elements[0][2] = 0.0f;
    result.elements[1][0] = 0.0f;
    result.elements[1][1] = diagonal;
    result.elements[1][2] = 0.0f;
    result.elements[2][0] = 0.0f;
    result.elements[2][1] = 0.0f;
    result.elements[2][2] = diagonal;
    return result;
}

/// Retrieves a transposed version of the specified matrix
Matrix3x3 matrix3x3_transpose(Matrix3x3 const *const matrix) {
    Matrix3x3 result;
    result.elements[0][0] = matrix->elements[0][0];
    result.elements[0][1] = matrix->elements[1][0];
    result.elements[0][2] = matrix->elements[2][0];
    result.elements[1][0] = matrix->elements[0][1];
    result.elements[1][1] = matrix->elements[1][1];
    result.elements[1][2] = matrix->elements[2][1];
    result.elements[2][0] = matrix->elements[0][2];
    result.elements[2][1] = matrix->elements[1][2];
    result.elements[2][2] = matrix->elements[2][2];
    return result;
}

/// Multiplies the two operands
Matrix3x3 matrix3x3_mul(Matrix3x3 const *const left, Matrix3x3 const *const right) {
    // clang-format off
    Matrix3x3 result;
    result.elements[0][0] = left->elements[0][0] * right->elements[0][0] + left->elements[0][1] * right->elements[1][0] + left->elements[0][2] * right->elements[2][0];
    result.elements[0][1] = left->elements[0][0] * right->elements[0][1] + left->elements[0][1] * right->elements[1][1] + left->elements[0][2] * right->elements[2][1];
    result.elements[0][2] = left->elements[0][0] * right->elements[0][2] + left->elements[0][1] * right->elements[1][2] + left->elements[0][2] * right->elements[2][2];
    result.elements[1][0] = left->elements[1][0] * right->elements[0][0] + left->elements[1][1] * right->elements[1][0] + left->elements[1][2] * right->elements[2][0];
    result.elements[1][1] = left->elements[1][0] * right->elements[0][1] + left->elements[1][1] * right->elements[1][1] + left->elements[1][2] * right->elements[2][1];
    result.elements[1][2] = left->elements[1][0] * right->elements[0][2] + left->elements[1][1] * right->elements[1][2] + left->elements[1][2] * right->elements[2][2];
    result.elements[2][0] = left->elements[2][0] * right->elements[0][0] + left->elements[2][1] * right->elements[1][0] + left->elements[2][2] * right->elements[2][0];
    result.elements[2][1] = left->elements[2][0] * right->elements[0][1] + left->elements[2][1] * right->elements[1][1] + left->elements[2][2] * right->elements[2][1];
    result.elements[2][2] = left->elements[2][0] * right->elements[0][2] + left->elements[2][1] * right->elements[1][2] + left->elements[2][2] * right->elements[2][2];
    // clang-format on
    return result;
}

/// Perform a chain rotation on matrices
Matrix3x3 matrix3x3_mul_chain(Matrix3x3 const *const chain, usize const count) {
    Matrix3x3 result = matrix3x3_diagonal(1.0);
    for (usize i = 0; i < count; ++i) {
        result = matrix3x3_mul(&result, chain + i);
    }
    return result;
}

/// Multiplies the matrix with the vector
Vector3 matrix3x3_mul_vector3(Matrix3x3 const *const left, Vector3 const *const right) {
    Vector3 result;
    result.x = left->elements[0][0] * right->x + left->elements[0][1] * right->y + left->elements[0][2] * right->z;
    result.y = left->elements[1][0] * right->x + left->elements[1][1] * right->y + left->elements[1][2] * right->z;
    result.z = left->elements[2][0] * right->x + left->elements[2][1] * right->y + left->elements[2][2] * right->z;
    return result;
}

/// Create a new rotation matrix
Matrix3x3 matrix3x3_rotation(RotationAxis const axis, f64 const angle) {
    f64 const cos_angle = math_cosine(angle);
    f64 const sin_angle = math_sine(angle);
    switch (axis) {
        case ROTATION_AXIS_X: {
            return (Matrix3x3) {
                .elements = { { 1.0, 0.0, 0.0 }, { 0.0, cos_angle, -sin_angle }, { 0.0, sin_angle, cos_angle } }
            };
        }
        case ROTATION_AXIS_Y: {
            return (Matrix3x3) {
                .elements = { { cos_angle, 0.0, sin_angle }, { 0.0, 1.0, 0.0 }, { -sin_angle, 0.0, cos_angle } }
            };
        }
        case ROTATION_AXIS_Z: {
            return (Matrix3x3) {
                .elements = { { cos_angle, -sin_angle, 0.0 }, { sin_angle, cos_angle, 0.0 }, { 0.0, 0.0, 1.0 } }
            };
        }
    }
    return matrix3x3_diagonal(1.0);
}

/// Computes the ecliptic drift since J2000
f64 ecliptic_drift(f64 const jc) {
    // Correct for drifting ecliptic due to planets pull on the Earth
    return 23.43929111 - (46.8150 + (0.00059 - 0.001813 * jc) * jc) * jc / 3600.0;
}

/// Creates a reference plane matrix from one reference plane to another
Matrix3x3 matrix3x3_reference_plane(ReferencePlane const from, ReferencePlane const to, f64 const at) {
    if (from == to) {
        return matrix3x3_diagonal(1.0);
    }

    Matrix3x3 rotation = matrix3x3_rotation(ROTATION_AXIS_X, ecliptic_drift(at));
    if (from == REFERENCE_PLANE_EQUATORIAL && to == REFERENCE_PLANE_ECLIPTIC) {
        return matrix3x3_transpose(&rotation);
    }
    return rotation;
}

/// Creates a matrix for the precession of coordinates
Matrix3x3 matrix3x3_precession(ReferencePlane const referencePlane, f64 const t1, f64 const t2) {
    f64 const dt = t2 - t1;
    switch (referencePlane) {
        case REFERENCE_PLANE_ECLIPTIC: {
            f64 capital_pi = math_radians(174.876383889);
            capital_pi += (((3289.4789 + 0.60622 * t1) * t1) + ((-869.8089 - 0.50491 * t1) + 0.03536 * dt) * dt) / ARCS;
            f64 pi = ((47.0029 - (0.06603 - 0.000598 * t1) * t1) + ((-0.03302 + 0.000598 * t1) + 0.00006 * dt) * dt);
            pi *= dt / ARCS;
            f64 pa = ((5029.0966 + (2.22226 - 0.000042 * t1) * t1) + ((1.11113 - 0.000042 * t1) - 0.000006 * dt) * dt);
            pa *= dt / ARCS;

            // clang-format off
            Matrix3x3 const chain[] = {
                matrix3x3_rotation(ROTATION_AXIS_Z, math_degrees(capital_pi + pa)),
                matrix3x3_rotation(ROTATION_AXIS_X, math_degrees(-pi)),
                matrix3x3_rotation(ROTATION_AXIS_Z, math_degrees(-capital_pi))
            };
            // clang-format on
            return matrix3x3_mul_chain(chain, ARRAY_SIZE(chain));
        }
        case REFERENCE_PLANE_EQUATORIAL: {
            // clang-format off
            f64 zeta = ((2306.2181 + (1.39646 - 0.000139 * t1) * t1) + ((0.30188 - 0.000344 * t1) + 0.017998 * dt) * dt);
            zeta *= dt / ARCS;
            f64 const z = zeta + ((0.7928 + 0.000411 * t1) + 0.000205 * dt) * dt * dt / ARCS;
            f64 theta = ((2004.3109 - (0.85330 + 0.000217 * t1) * t1) - ((0.42664 + 0.000217 * t1) + 0.041833 * dt) * dt);
            theta *= dt / ARCS;

            Matrix3x3 const chain[] = {
                matrix3x3_rotation(ROTATION_AXIS_Z, math_degrees(z)),
                matrix3x3_rotation(ROTATION_AXIS_Y, math_degrees(-theta)),
                matrix3x3_rotation(ROTATION_AXIS_Z, math_degrees(zeta))
            };
            // clang-format on
            return matrix3x3_mul_chain(chain, ARRAY_SIZE(chain));
        }
    }

    return matrix3x3_diagonal(1.0);
}

/// Transform equatorial coordinates to a Vector3
Vector3 vector3_from_equatorial(Equatorial const *const equatorial) {
    Vector3 result;
    result.x = equatorial->distance * math_cosine(equatorial->right_ascension) * math_cosine(equatorial->declination);
    result.y = equatorial->distance * math_sine(equatorial->right_ascension) * math_cosine(equatorial->declination);
    result.z = equatorial->distance * math_sine(equatorial->declination);
    return result;
}

/// Transform a Vector3 to equatorial coordinates
Equatorial equatorial_from_vector3(Vector3 const *const vector) {
    Equatorial result;
    result.right_ascension = math_arc_tangent2(vector->y, vector->x);
    result.declination = math_arc_tangent2(vector->z, math_sqrt(vector->x * vector->x + vector->y * vector->y));
    result.distance = math_sqrt(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
    return result;
}

/// Transforms Equatorial coordinates to Horizontal ones
Horizontal local_equatorial_to_horizontal(f64 const declination, f64 const hour_angle, f64 const latitude) {
    Equatorial equatorial;
    equatorial.right_ascension = hour_angle;
    equatorial.declination = declination;
    equatorial.distance = 1.0;

    Vector3 const position = vector3_from_equatorial(&equatorial);
    Matrix3x3 const latitude_transform = matrix3x3_rotation(ROTATION_AXIS_Y, -(90 - latitude));
    Vector3 const rotated = matrix3x3_mul_vector3(&latitude_transform, &position);

    // Add 180 to get the angle from north to east to south and so on
    Horizontal result;
    result.azimuth = math_arc_tangent2(rotated.y, rotated.x) + 180.0;
    result.altitude = math_arc_sine(rotated.z);
    return result;
}

/// Computes the Horizontal position of an object with spherical coordinates
Horizontal observe_geographic(Equatorial const *const equatorial,
                              Geographic const *const observer,
                              Time const *const date) {
    Time const utc = time_utc_local(date);
    f64 const lmst = time_gmst(&utc) + observer->longitude;
    f64 const local_hour_angle = lmst - equatorial->right_ascension;
    return local_equatorial_to_horizontal(equatorial->declination, local_hour_angle, observer->latitude);
}
