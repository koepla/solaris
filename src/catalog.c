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

#include <solaris/catalog.h>

#include "gen/objects.h"
#include "gen/planets.h"

/// Acquire the builtin catalog
Catalog catalog_acquire(void) {
    return (Catalog) { .planets = generated_planets,
                       .objects = generated_objects,
                       .planet_count = ARRAY_SIZE(generated_planets),
                       .object_count = ARRAY_SIZE(generated_objects) };
}

/// Compute the geographic position of the specified planet according to the spec
void compute_geographic_planet(MemoryArena *arena,
                               ComputeResult *result,
                               Planet const *const planet,
                               ComputeSpecification const *const spec) {
    result->altitudes = (f64 *) memory_arena_alloc(arena, spec->steps * sizeof(f64));
    result->azimuths = (f64 *) memory_arena_alloc(arena, spec->steps * sizeof(f64));
    result->count = spec->steps;

    Time it = spec->date;
    for (usize step = 0; step < spec->steps; ++step) {
        Equatorial const position_planet = planet_position_equatorial(planet, &it);
        Horizontal const position = observe_geographic(&position_planet, &spec->observer, &it);
        result->altitudes[step] = position.altitude;
        result->azimuths[step] = position.azimuth;
        time_add(&it, (s64) spec->step_size, spec->unit);
    }
}

/// Compute the geographic position of the specified fixed object according
/// to the specification
void compute_geographic_fixed(MemoryArena *arena,
                              ComputeResult *result,
                              Object const *const object,
                              ComputeSpecification const *const spec) {
    result->altitudes = (f64 *) memory_arena_alloc(arena, spec->steps * sizeof(f64));
    result->azimuths = (f64 *) memory_arena_alloc(arena, spec->steps * sizeof(f64));
    result->count = spec->steps;

    Time it = spec->date;
    for (usize step = 0; step < spec->steps; ++step) {
        Equatorial const position_object = object_position(object, &it);
        Horizontal const position = observe_geographic(&position_object, &spec->observer, &it);
        result->altitudes[step] = position.altitude;
        result->azimuths[step] = position.azimuth;
        time_add(&it, (s64) spec->step_size, spec->unit);
    }
}
