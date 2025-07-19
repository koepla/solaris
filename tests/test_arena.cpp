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

#include <gtest/gtest.h>
#include <solaris/arena.h>

TEST(MemoryArenaTest, CreateAndDestroy) {
    MemoryArena arena = memory_arena_identity(ALIGNMENT8);
    EXPECT_NE(arena.current, nullptr);
    EXPECT_GT(arena.current->size, 0u);

    memory_arena_destroy(&arena);
    EXPECT_EQ(arena.current, nullptr);
    EXPECT_EQ(arena.reserve, nullptr);
    EXPECT_EQ(arena.release, nullptr);
}

TEST(MemoryArenaTest, BasicAllocation) {
    MemoryArena arena = memory_arena_identity(ALIGNMENT8);
    void *ptr1 = memory_arena_alloc(&arena, 32);
    void *ptr2 = memory_arena_alloc(&arena, 64);

    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr1, ptr2);

    memory_arena_destroy(&arena);
}

TEST(MemoryArenaTest, BlockSplitting) {
    MemoryArena arena = memory_arena_identity(ALIGNMENT8);

    // Allocate more than 4 KiB to trigger a new block
    void *ptr1 = memory_arena_alloc(&arena, 8192);
    void *ptr2 = memory_arena_alloc(&arena, 16);

    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr1, ptr2);
    EXPECT_GE(arena.blocks, 2u);

    memory_arena_destroy(&arena);
}

TEST(MemoryArenaTest, ClearResetsBlocks) {
    MemoryArena arena = memory_arena_identity(ALIGNMENT8);
    void *ptr1 = memory_arena_alloc(&arena, 512);
    EXPECT_NE(ptr1, nullptr);
    EXPECT_EQ(arena.blocks, 1u);

    memory_arena_clear(&arena);
    void *ptr2 = memory_arena_alloc(&arena, 512);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_EQ(arena.blocks, 1u);

    memory_arena_destroy(&arena);
}

TEST(MemoryArenaTest, AlignmentCorrectness) {
    for (MemoryAlignment const alignment : { ALIGNMENT1, ALIGNMENT4, ALIGNMENT8 }) {
        MemoryArena arena = memory_arena_identity(alignment);
        for (usize i = 0; i < 10; ++i) {
            void *ptr = memory_arena_alloc(&arena, 1);
            EXPECT_EQ(reinterpret_cast<uintptr_t>(ptr) % alignment, 0u);
        }

        memory_arena_destroy(&arena);
    }
}
