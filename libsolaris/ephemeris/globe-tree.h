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

#ifndef SOLARIS_EPHEMERIS_GLOBE_TREE_H
#define SOLARIS_EPHEMERIS_GLOBE_TREE_H

#include <libsolaris/ephemeris/coordinates.h>

typedef struct GlobeSection {
    Geographic position;
    Geographic size;
} GlobeSection;

/// Create a new GlobeSection
/// @param position The position in the globe
/// @param size The angular size of the section
/// @return A new GlobeSection
SOLARIS_API GlobeSection globe_section_make(Geographic* position, Geographic* size);

/// Checks if the 'hay' GlobeSection contains the 'needle'
/// @param hay Containing GlobeSection
/// @param needle The needle in the hay;)
/// @return Boolean that indicates whether the 'hay' GlobeSection contains the 'needle'
SOLARIS_API b8 globe_section_contains(GlobeSection* hay, GlobeSection* needle);

/// Checks if the 'a' GlobeSection overlaps 'b'
/// @param a One GlobeSection
/// @param b Other GlobeSection
/// @return Boolean that indicates whether the two GlobeSections overlap
SOLARIS_API b8 globe_section_overlap(GlobeSection* a, GlobeSection* b);

#endif// SOLARIS_EPHEMERIS_GLOBE_TREE_H
