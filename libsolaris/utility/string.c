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

#include <assert.h>
#include <libsolaris/utility/string.h>

/// Creates a nil StringView
StringView string_view_nil() {
    return (StringView) { .data = nil, .length = 0 };;
}

/// Creates a new StringView instance
StringView string_view_new(const char* str, ssize length) {
    StringView result;
    result.data = str;
    result.length = length;
    return result;
}

/// Retrieves the length of a zero terminated C string
ssize zero_string_length(const char* str) {
    if (str == nil) {
        return -1;
    }

    ssize index = 0;
    while (str[index++] != 0) { }
    return index;
}

/// Creates a new StringView instance from a zero terminated C string
StringView string_view_from_native(const char* str) {
    return (StringView) { .data = str, .length = zero_string_length(str) };
}

/// Checks whether the provided StringViews are equal in terms of
b8 string_view_equal(StringView* left, StringView* right) {
    if (left->length != right->length) {
        return false;
    }

    for (ssize i = 0; i < left->length; ++i) {
        if (left->data[i] != right->data[i]) {
            return false;
        }
    }
    return true;
}

/// Creates a substring
StringView string_view_substring(StringView* view, ssize offset, ssize length) {
    if (offset + length > view->length) {
        assert(false && "runtime check failed: substring out of bounds!");
    }
    return string_view_new(view->data + offset, length);
}

/// Retrieves the leftmost index of the specified symbol in the view
ssize string_view_index_of(StringView* view, char symbol) {
    for (ssize i = 0; i < view->length; i++) {
        if (view->data[i] == symbol) {
            return i;
        }
    }
    return -1;
}
