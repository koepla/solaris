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

#ifndef SOLARIS_TYPES_H
#define SOLARIS_TYPES_H

#include <stdint.h>
#include <stddef.h>

#ifdef SOLARIS_SHARED
#ifdef _WIN32
#ifdef SOLARIS_BUILD
#define SOLARIS_API __declspec(dllexport)
#else
#define SOLARIS_API __declspec(dllimport)
#endif
#else
#define SOLARIS_API __attribute__((visibility("default")))
#endif
#else
#define SOLARIS_API
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef double f64;

typedef u64 usize;
typedef s64 ssize;

typedef u8 b8;
#define true 1
#define false 0

#define nil NULL

#define ARRAY_SIZE_IMPL(arr) sizeof(arr) / sizeof(arr[0])
#define ARRAY_SIZE(arr) ARRAY_SIZE_IMPL(arr)

#endif// SOLARIS_TYPES_H
