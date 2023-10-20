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

#ifndef SOLARIS_CATALOG_H
#define SOLARIS_CATALOG_H

#include <solaris/linear.h>
#include <solaris/math.h>
#include <solaris/object.h>
#include <solaris/planet.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Catalog {
    Planet* planets;
    Object* objects;
    usize planet_count;
    usize object_count;
} Catalog;

/// Acquire the builtin catalog
/// @return The builtin catalog
SOLARIS_API Catalog catalog_acquire(void);

typedef struct ComputeResult {
    f64* altitudes;
    f64* azimuths;
    usize capacity;
} ComputeResult;

typedef struct ComputeSpecification {
    Time date;
    Geographic observer;
    usize steps;
    usize step_size;
    TimeUnit unit;
} ComputeSpecification;

/// Compute the geographic position of the specified planet according
/// to the spec
/// @param arena The arena for the dynamic memory
/// @param result Computed result
/// @param planet The planet for the calculation
/// @param spec The compute spec
///
/// @note This function requires heap allocation, which is the reason
//        for the arena argument. If the capacity of the provided result is
//        not sufficient to hold the number of specified steps,
//        new memory gets allocated in the arena. It is highly recommended
//        to use a temporary arena in a real world scenario, due to the fact
//        of the need for a reallocation whenever a user increases the steps.
//        Another suggested approach would be to limit the number of steps
//        a user can specify, and pre-allocate the required space in result.
SOLARIS_API void compute_geographic_planet(MemoryArena* arena,
                                           ComputeResult* result,
                                           Planet* planet,
                                           ComputeSpecification* spec);

/// Compute the geographic position of the specified fixed object according
/// to the specification
/// @param arena The arena for the dynamic memory
/// @param result Computed result
/// @param object The object for the calculation
/// @param spec The compute specification
///
/// @note This function requires heap allocation, which is the reason
//        for the arena argument. If the capacity of the provided result is
//        not sufficient to hold the number of specified steps,
//        new memory gets allocated in the arena. It is highly recommended
//        to use a temporary arena in a real world scenario, due to the fact
//        of the need for a reallocation whenever a user increases the steps.
//        Another suggested approach would be to limit the number of steps
//        a user can specify, and pre-allocate the required space in result.
SOLARIS_API void compute_geographic_fixed(MemoryArena* arena,
                                          ComputeResult* result,
                                          Object* object,
                                          ComputeSpecification* spec);

#ifdef __cplusplus
}
#endif

#endif// SOLARIS_CATALOG_H
