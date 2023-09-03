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

#ifndef SOLARIS_EPHEMERIS_GENERATED_PLANETS_H
#define SOLARIS_EPHEMERIS_GENERATED_PLANETS_H

#include <libsolaris/ephemeris/planet.h>

static Planet generated_planets[] = {
    {
            PLANET_MERCURY,
            { 0.38709927, 0.20563593, 7.00497902, 252.2503235, 77.45779628, 48.33076593 },
            { 3.7e-07, 1.906e-05, -0.00594749, 149472.67411175, 0.16047689, -0.12534081 },
    },
    {
            PLANET_VENUS,
            { 0.72333566, 0.00677672, 3.39467605, 181.9790995, 131.60246718, 76.67984255 },
            { 3.9e-06, -4.107e-05, -0.0007889, 58517.81538729, 0.00268329, -0.27769418 },
    },
    {
            PLANET_MARS,
            { 1.52371034, 0.0933941, 1.84969142, -4.55343205, -23.94362959, 49.55953891 },
            { 1.847e-05, 7.882e-05, -0.00813131, 19140.30268499, 0.44441088, -0.29257343 },
    },
    {
            PLANET_JUPITER,
            { 5.202887, 0.04838624, 1.29861416, 34.33479152, 14.27495244, 100.29282654 },
            { -0.00011607, -0.00013253, -0.00322699, 3034.90371757, 0.18199196, 0.13024619 },
    },
    {
            PLANET_SATURN,
            { 9.54149883, 0.05550825, 2.49424102, 50.07571329, 92.86136063, 113.63998702 },
            { -3.065e-05, -0.00032044, 0.00451969, 1222.11494724, 0.54179478, -0.25015002 },
    },
    {
            PLANET_URANUS,
            { 19.18797948, 0.0468574, 0.77298127, 314.20276625, 172.43404441, 73.96250215 },
            { -0.00020455, -1.55e-05, -0.00180155, 428.49512595, 0.09266985, 0.05739699 },
    },
    {
            PLANET_NEPTUNE,
            { 30.06952752, 0.00895439, 1.7700552, 304.22289287, 146.68158724, 131.78635853 },
            { 6.447e-05, 8.18e-06, 0.000224, 218.46515314, 0.01009938, -0.00606302 },
    },
};

#elif
#error "Generated planets should only be included once, they are for internal use only!"
#endif// SOLARIS_EPHEMERIS_GENERATED_PLANETS_H
