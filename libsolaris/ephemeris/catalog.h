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

#ifndef SOLARIS_EPHEMERIS_CATALOG_H
#define SOLARIS_EPHEMERIS_CATALOG_H

#include <libsolaris/ephemeris/coordinates.h>
#include <libsolaris/ephemeris/fixed-object.h>
#include <libsolaris/ephemeris/planet.h>
#include <libsolaris/math.h>

typedef struct CatalogCollection {
    MemoryArena* arena;
    Planet* planets;
    FixedObject* objects;
} CatalogCollection;

/// Create a new catalog collection
/// @param arena The arena where the objects in the catalog get stored
/// @return An empty catalog collection
///
/// @note The catalog takes sort of ownership of the provided arena,
///       in a sense that it is used for storing the objects. It is not
///       upon the catalog to free the arena, this task is up to the
///       user of the catalog, who provides the arena. The catalog
///       objects die together with the arena, as this is the
///       place where they reside in memory
CatalogCollection catalog_collection_new(MemoryArena* arena);

/// Decode a model file of the NGC format
/// @param catalog The catalog which stores the decoded entries
/// @param model The model file
///
/// @note The model must adhere to the NGC format, which is described on the
///       following web page: https://cdsarc.cds.unistra.fr/ftp/VII/118/ReadMe.
///       Any other models are currently unsupported, it is up to the user
///       to manually load other models.
void catalog_collection_decode(CatalogCollection* catalog, StringView* model);

typedef struct ComputeResult {
    f64* altitudes;
    f64* azimuths;
    usize capacity;
} ComputeResult;

typedef struct ComputeSpecification {
    DateTime date;
    Geographic observer;
    usize steps;
    usize step_size;
    DateTimeUnit unit;
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
void compute_geographic_planet(MemoryArena* arena, ComputeResult* result, Planet* planet, ComputeSpecification* spec);

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
void compute_geographic_fixed(MemoryArena* arena,
                              ComputeResult* result,
                              FixedObject* object,
                              ComputeSpecification* spec);

#endif// SOLARIS_EPHEMERIS_CATALOG_H
