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
#include <stdlib.h>


int arena_alloc(int argc, char** argv) {
    MemoryArena arena = memory_arena_new(
            &(MemoryArenaSpecification){ .alignment = ALIGNMENT8, .reserve = malloc, .release = free });

    for (usize i = 0; i < 30; ++i) {
        memory_arena_alloc(&arena, 200);
    }
    for (usize i = 0; i < 2; ++i) {
        memory_arena_alloc(&arena, 6000);
    }

    // Lets request one gig like a real mad man 😎
    memory_arena_alloc(&arena, (usize) 1 * 1024 * 1024 * 1024);
    memory_arena_destroy(&arena);
    return 69;
}
