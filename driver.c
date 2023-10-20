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
#include <stdio.h>

#include <solaris/solaris.h>
#include "solaris/globe.h"

#define test(test, condition) assert("test " test " failed" && (condition))
#define passed() printf("[+] passed tests for %s\n", __FUNCTION__)

static b8 eq_f64(f64 a, f64 b) {
    return math_abs(a - b) < 1e12;
}

static b8 eq_gp(GlobePosition a, f64 right_ascension, f64 declination) {
    return eq_f64(a.right_ascension, right_ascension) && eq_f64(a.declination, declination);
}

static void test_globe_position() {
    GlobePosition origin = globe_position_make(0.0, 0.0);
    test("origin", eq_gp(origin, 0.0, 0.0));

    GlobePosition decl_positive = globe_position_make(180.0, 45.0);
    test("decl_positive", eq_gp(decl_positive, 180.0, 45.0));

    GlobePosition decl_negative = globe_position_make(180.0, -45.0);
    test("decl_negative", eq_gp(decl_negative, 180.0, -45.0));

    GlobePosition decl_overshoot = globe_position_make(180.0, 95.0);
    test("decl_overshoot", eq_gp(decl_overshoot, 0.0, 85.0));

    GlobePosition decl_undershoot = globe_position_make(180.0, -95.0);
    test("decl_overshoot", eq_gp(decl_undershoot, 0.0, -85.0));

    GlobePosition ra_positive = globe_position_make(275.0, 50.0);
    test("ra_positive", eq_gp(ra_positive, 275.0, 50.0));

    GlobePosition ra_negative = globe_position_make(-270.0, 50.0);
    test("ra_negative", eq_gp(ra_negative, 90.0, 50.0));

    GlobePosition ra_overshoot = globe_position_make(370.0, 10.0);
    test("ra_overshoot", eq_gp(ra_overshoot, 10.0, 10.0));

    GlobePosition both_overshoot = globe_position_make(450.0, 95.0);
    test("both_overshoot", eq_gp(both_overshoot, 270.0, -85.0));

    GlobePosition both_undershoot = globe_position_make(-90.0, -95.0);
    test("both_undershoot", eq_gp(both_undershoot, 90.0, -85.0));

    GlobePosition lower_limit = globe_position_make(0.0, -90.0);
    test("lower_limit", eq_gp(lower_limit, 0.0, -90.0));

    GlobePosition upper_limit = globe_position_make(360.0, 90.0);
    test("upper_limit", eq_gp(upper_limit, 360.0, 90.0));
    passed();
}

static void test_globe_tree(MemoryArena* arena) {
    // create the root tree
    GlobeTree* tree = globe_tree_make_root(arena);
    test("globe_tree_make", tree != nil);

    // test if all objects are inside
    usize size = globe_tree_size(tree);
    test("globe_tree_size", size == catalog_acquire().object_count);

    GlobeSection small_section = globe_section_make((GlobePosition){ 0.0, 0.0 }, (GlobeArea){ 2.0, 2.0 });
    GlobeNodeList small_result = globe_tree_search(tree, arena, &small_section);
    test("globe_tree_search_small", small_result.size > 0);

    GlobeSection all_section = globe_section_make((GlobePosition){ 0.0, -90.0 }, (GlobeArea){ 360.0, 180.0 });
    GlobeNodeList all_result = globe_tree_search(tree, arena, &all_section);
    test("globe_tree_search_all", all_result.size == size);
    passed();
}

int main(int argc, char** argv) {
    MemoryArena test_arena = memory_arena_identity(ALIGNMENT8);
    test_globe_position();
    test_globe_tree(&test_arena);
    memory_arena_destroy(&test_arena);
    return 0;
}
