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

#include <examples/catalog/decode.h>
#include <examples/common.h>

int catalog_decode(int argc, char** argv) {
    const char* catalog_path = NULL;
    for (int i = 0; i < argc; ++i) {
        if (zero_string_equal(argv[i], "--catalog") && i < argc) {
            catalog_path = argv[i + 1];
        }
    }

    if (catalog_path == NULL) {
        return -1;
    }

    MemoryArenaSpecification spec;
    spec.alignment = ALIGNMENT8;
    spec.reserve = malloc;
    spec.release = free;
    MemoryArena arena = memory_arena_new(&spec);

    String model = file_read(&arena, catalog_path);
    StringView model_view = string_view_from_string(&model);
    CatalogCollection catalog = catalog_collection_new(&arena);
    catalog_collection_decode(&catalog, &model_view);

    memory_arena_destroy(&arena);
    return 0;
}
