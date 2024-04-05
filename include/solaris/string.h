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

#ifndef SOLARIS_STRING_H
#define SOLARIS_STRING_H

#include <solaris/arena.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct StringView {
    const char* data;
    ssize length;
} StringView;

/// Creates a nil StringView
SOLARIS_API StringView string_view_nil(void);

/// Creates a new StringView instance
/// @param str The string
/// @param length The length of the string
/// @return StringView instance
SOLARIS_API StringView string_view_make(const char* str, ssize length);

/// Creates a new StringView instance from a zero terminated C string
/// @param str The string
/// @return StringView instance
SOLARIS_API StringView string_view_from_native(const char* str);

/// Checks whether the provided StringViews are equal in terms of
/// length and data
/// @param left
/// @param right
/// @return Boolean that indicates equality
SOLARIS_API b8 string_view_equal(StringView* left, StringView* right);

/// Checks whether the provided hay StringView contains the needle, without case sensitivity
/// @param hay The hay which is searched for the needle
/// @param needle The needle which is searched in the hay
/// @return Boolean that indicates whether the hay contains the needle
SOLARIS_API b8 string_view_contains(StringView* hay, StringView* needle);

/// Creates a substring
/// @param view The view that contains the substring
/// @param offset The offset where the substring should start
/// @param length The length of the substring
/// @return A substring
SOLARIS_API StringView string_view_substring(StringView* view, ssize offset, ssize length);

/// Retrieves the leftmost index of the specified symbol in the view
/// @param view The view for the index search
/// @param symbol The symbol to search for
/// @return The index if the symbol was found, -1 if there are zero occurrences
SOLARIS_API ssize string_view_index_of(StringView* view, char symbol);

#ifdef __cplusplus
}
#endif

#endif// SOLARIS_STRING_H
