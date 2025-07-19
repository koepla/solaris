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

#ifndef SOLARIS_LINEAR_H
#define SOLARIS_LINEAR_H

#include <solaris/time.h>
#include <solaris/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Equatorial coordinates hold spherical positional information
typedef struct Equatorial {
    f64 right_ascension;
    f64 declination;
    f64 distance;
} Equatorial;

/// Horizontal coordinates are spherical coordinates in
/// the observers horizontal system
typedef struct Horizontal {
    f64 azimuth;
    f64 altitude;
} Horizontal;

/// Geographic coordinates describe an observers
/// position on the globe of the earth
typedef struct Geographic {
    f64 latitude;
    f64 longitude;
} Geographic;

/// Vector3 describes cartesian coordinates in
/// 3-dimensional space
typedef struct Vector3 {
    f64 x;
    f64 y;
    f64 z;
} Vector3;

/// Retrieves the length of the vector
/// @param vector The vector
/// @return Length
SOLARIS_API f64 vector3_length(Vector3 const *vector);

/// Adds the two vector operands
/// @param left The left operand
/// @param right The right operand
/// @return Result of addition
SOLARIS_API Vector3 vector3_add(Vector3 const *left, Vector3 const *right);

/// Subtracts the two vector operands
/// @param left The left operand
/// @param right The right operand
/// @return Result of subtraction
SOLARIS_API Vector3 vector3_sub(Vector3 const *left, Vector3 const *right);

/// Matrix3x3 is self-documenting ;)
typedef struct Matrix3x3 {
    f64 elements[3][3];
} Matrix3x3;

/// Creates a diagonal matrix
/// @param diagonal The diagonal value
/// @return The matrix
SOLARIS_API Matrix3x3 matrix3x3_diagonal(f64 diagonal);

/// Retrieves a transposed version of the specified matrix
/// @param matrix The source matrix
/// @return Transposed matrix
SOLARIS_API Matrix3x3 matrix3x3_transpose(Matrix3x3 const *matrix);

/// Multiplies the two operands
/// @param left The left operand
/// @param right The right operand
/// @return The multiplication result
SOLARIS_API Matrix3x3 matrix3x3_mul(Matrix3x3 const *left, Matrix3x3 const *right);

/// Perform a chain rotation on matrices
/// @param chain The matrix chain
/// @param count The matrix count in the chain
/// @return The multiplication result
SOLARIS_API Matrix3x3 matrix3x3_mul_chain(Matrix3x3 const *chain, usize count);

/// Multiplies the matrix with the vector
/// @param left The matrix
/// @param right The vector
/// @return The multiplication result
SOLARIS_API Vector3 matrix3x3_mul_vector3(Matrix3x3 const *left, Vector3 const *right);

/// Represents an axis of rotation in 3-dimensional space
typedef enum RotationAxis { ROTATION_AXIS_X, ROTATION_AXIS_Y, ROTATION_AXIS_Z } RotationAxis;

/// Create a new rotation matrix
/// @param axis Axis to rotate around
/// @param angle Angle
/// @return
SOLARIS_API Matrix3x3 matrix3x3_rotation(RotationAxis axis, f64 angle);

/// Computes the ecliptic drift since J2000
/// @param jc Julian Centuries since J2000
/// @return ecliptic drift in math_degrees
SOLARIS_API f64 ecliptic_drift(f64 jc);

/// Represents a plane of reference in astrometry
typedef enum ReferencePlane { REFERENCE_PLANE_EQUATORIAL, REFERENCE_PLANE_ECLIPTIC } ReferencePlane;

/// Creates a reference plane matrix from one reference plane to another
/// @param from Initial reference plane
/// @param to Reference frame that we want to transform to
/// @param at Time in julian centuries since J2000
/// @return transformation matrix
SOLARIS_API Matrix3x3 matrix3x3_reference_plane(ReferencePlane from, ReferencePlane to, f64 at);

/// Creates a matrix for the precession of coordinates
/// @param referencePlane Plane of reference
/// @param t1 Current epoch
/// @param t2 Epoch to that we want to precess
/// @note `t1` and `t2` are both in julian centuries since j2000
/// @return transformation matrix
SOLARIS_API Matrix3x3 matrix3x3_precession(ReferencePlane referencePlane, f64 t1, f64 t2);

/// Transform equatorial coordinates to a Vector3
/// @param equatorial The equatorial coordinates
/// @return The transformed rectangular coordinates
SOLARIS_API Vector3 vector3_from_equatorial(Equatorial const *equatorial);

/// Transform a Vector3 to equatorial coordinates
/// @param vector The rectangular coordinates
/// @return The transformed spherical coordinates
SOLARIS_API Equatorial equatorial_from_vector3(Vector3 const *vector);

/// Transforms Equatorial coordinates to Horizontal ones
/// @param declination declination in math_degrees
/// @param hour_angle Hour angle (by Greenwich Mean Sidereal Time) in degrees
/// @param latitude latitude in math_degrees
/// @return The transformed horizontal coordinates
SOLARIS_API Horizontal local_equatorial_to_horizontal(f64 declination, f64 hour_angle, f64 latitude);

/// Computes the Horizontal position of an object with spherical coordinates
/// @param equatorial The spherical coordinates of the object
/// @param observer The geographic coordinates of the observer
/// @param date The date and time for the computation
/// @return the Computed horizontal coordinates
SOLARIS_API Horizontal observe_geographic(Equatorial const *equatorial, Geographic const *observer, Time const *date);

#ifdef __cplusplus
}
#endif

#endif// SOLARIS_LINEAR_H
