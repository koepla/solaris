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

#include <examples/arena/alloc.h>
#include <examples/catalog/decode.h>
#include <examples/common.h>

typedef int (*ExampleStartup)(int, char**);

typedef struct Example {
    const char* name;
    ExampleStartup startup;
} Example;

// clang-format off
static Example examples[] = {
    { .name = "arena:alloc", .startup = arena_alloc },
    { .name = "catalog:decode", .startup = catalog_decode },
};
// clang-format on

int start_example(const char* example, int argc, char** argv) {
    for (size_t i = 0; i < ARRAY_SIZE(examples); ++i) {
        Example* current = examples + i;
        if (zero_string_equal(current->name, example)) {
            return current->startup(argc, argv);
        }
    }
    fprintf(stderr, "no example named '%s'!\n", example);
    return EXIT_FAILURE;
}

const char* usage_format =
        "solaris example runner - Copyright (c) Elias Engelbert Plank\n"
        "usage: %s\n"
        "[ --help ]\n"
        "[ --sample <name> [, <args...> ]\n";

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, usage_format, argv[0]);
        exit(64);
    }
    for (int i = 0; i < argc; ++i) {
        if (zero_string_equal(argv[i], "--sample") && i < argc) {
            return start_example(argv[i + 1], argc - 3, argv + 3);
        } else if (zero_string_equal(argv[i], "--help")) {
            printf(usage_format, argv[0]);
        }
    }
    return 0;
}
