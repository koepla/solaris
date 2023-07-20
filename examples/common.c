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

#include <examples/common.h>

/// Checks if the two zero terminated strings are equal
b8 zero_string_equal(const char* a, const char* b) {
    StringView left = string_view_new_zero(a);
    StringView right = string_view_new_zero(b);
    return string_view_equal(&left, &right);
}

/// Reads a file and returns the content as a string
String file_read(MemoryArena* arena, const char* name) {
    FILE* file = fopen(name, "rb");
    if (file == NULL) {
        return string_null();
    }

    ssize file_size;
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    String content = string_new(arena, file_size + 1);
    content.base[file_size] = 0;

    fread(content.base, sizeof(char), file_size, file);
    fclose(file);

    return content;
}