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

#ifndef SOLARIS_PLANET_H
#define SOLARIS_PLANET_H

#include <solaris/linear.h>
#include <solaris/math.h>
#include <solaris/string.h>
#include <solaris/time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum PlanetName {
    PLANET_MERCURY,
    PLANET_VENUS,
    PLANET_EARTH,
    PLANET_MARS,
    PLANET_JUPITER,
    PLANET_SATURN,
    PLANET_URANUS,
    PLANET_NEPTUNE,
    PLANET_COUNT
} PlanetName;

typedef struct Elements {
    f64 semi_major_axis;
    f64 eccentricity;
    f64 inclination;
    f64 mean_longitude;
    f64 lon_perihelion;
    f64 lon_asc_node;
} Elements;

typedef struct Planet {
    PlanetName name;
    Elements state;
    Elements rate;
} Planet;

/// Computes the orbital position of the planet
/// @param planet The planet
/// @param date date and time for the computation
/// @return the computed orbital coordinates
SOLARIS_API Elements planet_position_orbital(Planet* planet, Time* date);

/// Computes the equatorial position of the planet
/// @param planet The planet
/// @param date date and time for the computation
/// @return the computed equatorial coordinates
SOLARIS_API Equatorial planet_position_equatorial(Planet* planet, Time* date);

/// Retrieves the name of the planet in string representation
/// @param name The name of the planet
/// @return The name in string representation
SOLARIS_API const char* planet_string(PlanetName name);

#ifdef __cplusplus
}
#endif

#endif// SOLARIS_PLANET_H
