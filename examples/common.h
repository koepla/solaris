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

#ifndef SOLARIS_EXAMPLES_COMMON_H
#define SOLARIS_EXAMPLES_COMMON_H

#include <libsolaris/utility/string.h>

#include <stdio.h>
#include <stdlib.h>

/// Checks if the two zero terminated strings are equal
/// @param a First string
/// @param b Second string
/// @return Boolean that indicates equality
b8 zero_string_equal(const char* a, const char* b);

/// Reads a file and returns the content as a string
/// @param arena Arena for the allocation
/// @param name Name of the file
/// @return Content or null String
String file_read(MemoryArena* arena, const char* name);

#endif// SOLARIS_EXAMPLES_COMMON_H