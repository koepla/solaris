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

#ifndef SOLARIS_ARENA_H
#define SOLARIS_ARENA_H

#include <solaris/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*MemoryReserveFunc)(usize);
typedef void (*MemoryReleaseFunc)(void*);

typedef enum MemoryAlignment { ALIGNMENT1 = 1, ALIGNMENT4 = 4, ALIGNMENT8 = 8 } MemoryAlignment;

typedef struct MemoryArenaSpecification {
    MemoryAlignment alignment;
    MemoryReserveFunc reserve;
    MemoryReleaseFunc release;
} MemoryArenaSpecification;

typedef struct MemoryBlock {
    usize size;
    usize used;
    u8* base;
    struct MemoryBlock* before;
    usize id;
} MemoryBlock;

typedef struct MemoryArena {
    MemoryBlock* current;
    MemoryAlignment alignment;
    MemoryReserveFunc reserve;
    MemoryReleaseFunc release;
    usize blocks;
    usize total_memory;
} MemoryArena;

/// Creates a new memory arena
/// @param arena The arena
/// @param spec The arena specification
/// @return Memory arena with one block
SOLARIS_API MemoryArena memory_arena_make(MemoryArenaSpecification* spec);

/// Creates an identity memory arena
/// @param alignment The alignment for the allocations
/// @return Identity memory arena with one block
///
/// @note An identity arena is an arena with malloc and free
///       as reserve/release functions
SOLARIS_API MemoryArena memory_arena_identity(MemoryAlignment alignment);

/// Clears the memory arena by freeing all blocks
/// @param arena The arena
SOLARIS_API void memory_arena_clear(MemoryArena* arena);

/// Destroys the specified memory arena
/// @param arena The arena
SOLARIS_API void memory_arena_destroy(MemoryArena* arena);

/// Allocate a block of memory in the specified arena
/// @param arena The arena
/// @param size The size of the requested block
/// @return Memory u8*
SOLARIS_API u8* memory_arena_alloc(MemoryArena* arena, usize size);

#ifdef __cplusplus
}
#endif

#endif// SOLARIS_ARENA_H
