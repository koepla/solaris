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

#include <stdlib.h>

#include <solaris/arena.h>

enum {
    BLOCK_SIZE = 4 * 1024,
};

/// Align the specified size according to arena alignment
static usize memory_arena_alignment_size(MemoryArena const *const arena, usize size) {
    usize const alignment = (usize) arena->alignment;
    return (size + alignment - 1) & ~(alignment - 1);
}

/// Align the used offset according to arena alignment
static usize memory_arena_alignment_offset(MemoryArena const *const arena) {
    return memory_arena_alignment_size(arena, arena->current->used);
}

/// Creates a new memory block
static MemoryBlock *memory_arena_block_new(MemoryArena *const arena, usize const requested_size) {
    // At this point, the requested size is already aligned
    usize const actual_size = requested_size > BLOCK_SIZE ? requested_size : BLOCK_SIZE;

    MemoryBlock *block = arena->reserve(sizeof(MemoryBlock) + actual_size);
    block->base = (u8 *) block + sizeof(MemoryBlock);
    block->size = actual_size;
    block->used = 0;
    block->before = nil;
    block->id = arena->blocks++;
    arena->total_memory += BLOCK_SIZE;
    return block;
}

/// Creates a new memory arena
MemoryArena memory_arena_make(MemoryArenaSpecification const *const spec) {
    MemoryArena result;
    result.alignment = spec->alignment;
    result.reserve = spec->reserve;
    result.release = spec->release;
    result.blocks = 0;
    result.total_memory = 0;
    result.current = memory_arena_block_new(&result, 0);
    return result;
}

/// Creates an identity memory arena
MemoryArena memory_arena_identity(MemoryAlignment const alignment) {
    MemoryArenaSpecification spec;
    spec.alignment = alignment;
    spec.reserve = malloc;
    spec.release = free;
    return memory_arena_make(&spec);
}

/// Clears the memory arena by freeing all blocks
void memory_arena_clear(MemoryArena *const arena) {
    for (MemoryBlock *it = arena->current; it != nil; it = it->before) {
        arena->release(it);
    }
    arena->blocks = 0;
    arena->total_memory = 0;
    arena->current = memory_arena_block_new(arena, 0);
}

/// Destroys the specified memory arena
void memory_arena_destroy(MemoryArena *const arena) {
    while (arena->current != nil) {
        MemoryBlock *before = arena->current->before;
        // We must release the memory block itself as it is the base of the allocation
        arena->release(arena->current);
        arena->current = before;
    }
    arena->reserve = nil;
    arena->release = nil;
    arena->current = nil;
    arena->blocks = 0;
    arena->total_memory = 0;
}

/// Allocate a block of memory in the specified arena
void *memory_arena_alloc(MemoryArena *const arena, usize const size) {
    usize const aligned_size = memory_arena_alignment_size(arena, size);

    if (arena->current->used + aligned_size > arena->current->size) {
        // Not enough space → add new block and prepend to list
        MemoryBlock *new_block = memory_arena_block_new(arena, aligned_size);
        new_block->before = arena->current;
        arena->current = new_block;
    }

    usize const offset = memory_arena_alignment_offset(arena);
    void *result = arena->current->base + offset;
    arena->current->used = offset + aligned_size;

    return result;
}
