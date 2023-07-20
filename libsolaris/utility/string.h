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

#ifndef SOLARIS_UTILITY_STRING_H
#define SOLARIS_UTILITY_STRING_H

#include <libsolaris/utility/arena.h>

typedef struct String {
    char* base;
    ssize length;
} String;

/// Creates a null String
String string_null();

/// Creates a new String instance
/// @param arena The arena for the allocation
/// @param length The length of the string
/// @return String instance
///
/// @note Strings are heap allocated, which is the reason
///       why an arena is necessary. For views to constant
///       strings see StringView
String string_new(MemoryArena* arena, ssize length);

typedef struct StringView {
    const char* data;
    ssize length;
} StringView;

/// Creates a null StringView
StringView string_view_null();

/// Creates a new StringView instance
/// @param str The string
/// @param length The length of the string
/// @return StringView instance
StringView string_view_new(const char* str, ssize length);

/// Creates a new StringView instance from a zero terminated C string
/// @param str The string
/// @return StringView instance
StringView string_view_new_zero(const char* str);

/// Creates a new StringView instance from a String
/// @param string The string
/// @return StringView instance
StringView string_view_from_string(String* string);

/// Checks whether the provided StringViews are equal in terms of
/// length and data
/// @param left
/// @param right
/// @return Boolean that indicates equality
b8 string_view_equal(StringView* left, StringView* right);

/// Creates a substring
/// @param view The view that contains the substring
/// @param offset The offset where the substring should start
/// @param length The length of the substring
/// @return A substring
StringView string_view_substring(StringView* view, ssize offset, ssize length);

/// Retrieves the leftmost index of the specified symbol in the view
/// @param view The view for the index search
/// @param symbol The symbol to search for
/// @return The index if the symbol was found, -1 if there are zero occurrences
ssize string_view_index_of(StringView* view, char symbol);

#endif// SOLARIS_UTILITY_STRING_H