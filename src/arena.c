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

/// Align the specified size according to arena alignment
usize memory_arena_alignment_size(MemoryArena* arena, usize size) {
    usize align_distance = size % arena->alignment;
    if (align_distance != 0) {
        size += align_distance;
    }
    return size;
}

/// Align the used offset according to arena alignment
usize memory_arena_alignment_offset(MemoryArena* arena) {
    return memory_arena_alignment_size(arena, arena->current->used);
}

MemoryBlock* memory_arena_block_new(MemoryArena* arena, usize requested_size) {
    // Default block size is 4 Kb, which should be a page?
    usize block_size = 4 * 1024;

    // At this point, the requested size is already aligned
    usize actual_size = requested_size > block_size ? requested_size : block_size;

    MemoryBlock* block = (MemoryBlock*) arena->reserve(sizeof(MemoryBlock) + actual_size);
    block->base = (u8*) block + sizeof(MemoryBlock);
    block->size = actual_size;
    block->used = 0;
    block->before = nil;
    block->id = arena->blocks++;
    arena->total_memory += block_size;
    return block;
}

/// Creates a new memory arena
MemoryArena memory_arena_make(MemoryArenaSpecification* spec) {
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
MemoryArena memory_arena_identity(MemoryAlignment alignment) {
    MemoryArenaSpecification spec;
    spec.alignment = alignment;
    spec.reserve = malloc;
    spec.release = free;
    return memory_arena_make(&spec);
}

/// Destroys the specified memory arena
void memory_arena_destroy(MemoryArena* arena) {
    while (arena->current != nil) {
        MemoryBlock* before = arena->current->before;
        // We must release the memory block itself as it is the base of the allocation
        arena->release(arena->current);
        arena->current = before;
    }
    arena->reserve = nil;
    arena->release = nil;
    arena->current = nil;
}

/// Allocate a block of memory in the specified arena
u8* memory_arena_alloc(MemoryArena* arena, usize size) {
    usize aligned_size = memory_arena_alignment_size(arena, size);

    b8 back_swap = false;
    if (arena->current->used + aligned_size > arena->current->size) {
        // If the current block is less than half full, we prepend a new block
        // for the incoming allocation to the old current block, to not waste the unused memory
        MemoryBlock* block = memory_arena_block_new(arena, aligned_size);
        if (arena->current->used < arena->current->size / 2) {
            back_swap = true;
        }
        block->before = arena->current;
        arena->current = block;
    }

    usize aligned_offset = memory_arena_alignment_offset(arena);
    u8* result = arena->current->base + aligned_offset;
    arena->current->used = aligned_offset + aligned_size;

    if (back_swap) {
        MemoryBlock* current = arena->current;
        MemoryBlock* previous_before = current->before->before;
        arena->current = arena->current->before;
        arena->current->before = current;
        arena->current->before->before = previous_before;
    }

    return result;
}
