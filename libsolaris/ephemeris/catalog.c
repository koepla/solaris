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

#include <libsolaris/ephemeris/catalog.h>
#include <stdio.h>

/// Create a new catalog collection
CatalogCollection catalog_collection_new(MemoryArena* arena) {
    CatalogCollection result = { .arena = arena, .objects = NULL, .planets = NULL };
    return result;
}

typedef struct LineIterator {
    StringView source;
    ssize offset;
} LineIterator;

/// Creates a new line iterator from a source string
LineIterator line_iterator_make(StringView* source) {
    LineIterator result = { .source = *source, .offset = 0 };
    return result;
}

/// Counts the number of total new lines
ssize line_iterator_count(LineIterator* it) {
    ssize count = 1;
    for (ssize i = 0; i < it->source.length; ++i) {
        if (it->source.data[i] == '\n') {
            count++;
        }
    }
    return count;
}

/// Retrieves the remaining part of the line iterator source
StringView line_iterator_remaining(LineIterator* it) {
    return string_view_substring(&it->source, it->offset, it->source.length - it->offset);
}

/// Retrieves the next line
b8 line_iterator_next(LineIterator* it, StringView* next) {
    StringView hay = line_iterator_remaining(it);
    ssize new_line_index = string_view_index_of(&hay, '\n');
    if (new_line_index == -1) {
        return false;
    }
    next->data = hay.data;
    next->length = new_line_index;
    it->offset += new_line_index + 1;
    return true;
}

/// Decode a model file of the NGC format
void catalog_collection_decode(CatalogCollection* catalog, StringView* model) {
    LineIterator iterator = line_iterator_make(model);
    usize count = line_iterator_count(&iterator);
    catalog->objects = (FixedObject*) memory_arena_alloc(catalog->arena, count * sizeof(FixedObject));

    printf("[catalog]: expecting %lld lines\n", count);
    for (StringView line = { 0 }; line_iterator_next(&iterator, &line);) {
        printf("[catalog]: parsing line %.*s\n", (int) line.length, line.data);
    }
}

/// Compute the geographic position of the specified planet according
/// to the spec
void compute_geographic_planet(MemoryArena* arena, ComputeResult* result, Planet* planet, ComputeSpecification* spec) {
    if (result->capacity < spec->steps) {
        result->altitudes = (f64*) memory_arena_alloc(arena, spec->steps * sizeof(f64));
        result->azimuths = (f64*) memory_arena_alloc(arena, spec->steps * sizeof(f64));
        result->capacity = spec->steps;
    }
    for (usize step = 0; step < spec->steps; ++step) {
        Equatorial position_planet = planet_position(planet, &spec->date);
        Horizontal position = observe_geographic(&position_planet, &spec->observer, &spec->date);
        result->altitudes[step] = position.altitude;
        result->azimuths[step] = position.azimuth;
        date_time_add(&spec->date, (s64) spec->step_size, spec->unit);
    }
}

/// Compute the geographic position of the specified fixed object according
/// to the specification
void compute_geographic_fixed(MemoryArena* arena,
                              ComputeResult* result,
                              FixedObject* object,
                              ComputeSpecification* spec) {
    if (result->capacity < spec->steps) {
        result->altitudes = (f64*) memory_arena_alloc(arena, spec->steps);
        result->azimuths = (f64*) memory_arena_alloc(arena, spec->steps);
        result->capacity = spec->steps;
    }
    for (usize step = 0; step < spec->steps; ++step) {
        Equatorial position_object = fixed_object_position(object, &spec->date);
        Horizontal position = observe_geographic(&position_object, &spec->observer, &spec->date);
        result->altitudes[step] = position.altitude;
        result->azimuths[step] = position.azimuth;
        date_time_add(&spec->date, (s64) spec->step_size, spec->unit);
    }
}
