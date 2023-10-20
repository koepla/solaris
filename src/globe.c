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

#include <solaris/catalog.h>
#include <solaris/globe.h>
#include <solaris/math.h>

#include <assert.h>

/// Creates a new GlobeArea
GlobeArea globe_area_make(f64 width, f64 height) {
    return (GlobeArea){ .width = width, .height = height };
}

static void globe_position_clamp(GlobePosition* position) {
    f64 wrapped_declination = math_bound(position->declination, -90.0, 90.0);
    if (wrapped_declination != position->declination) {
        position->right_ascension = math_modulo(position->right_ascension + 180.0, 360.0);
        position->declination = -wrapped_declination;
    }
}

/// Create a new GlobePosition
GlobePosition globe_position_make(f64 right_ascension, f64 declination) {
    GlobePosition result = { right_ascension, declination };
    globe_position_clamp(&result);
    return result;
}

/// Create a new GlobeSection
GlobeSection globe_section_make(GlobePosition position, GlobeArea size) {
    return (GlobeSection){ .position = position, .size = size };
}

/// Checks if the 'hay' GlobeSection contains the 'needle'
b8 globe_section_contains(GlobeSection* hay, GlobeSection* needle) {
    // here we do not actually want the clamping of globe_position_make, since it makes it a lot easier
    // clang-format off
    GlobePosition hay_end = (GlobePosition) {
        hay->position.right_ascension + hay->size.width,
        hay->position.declination + hay->size.height
    };

    GlobePosition needle_end = (GlobePosition) {
        needle->position.right_ascension + needle->size.width,
        needle->position.declination + needle->size.height
    };
    // clang-format on

    return hay->position.right_ascension <= needle->position.right_ascension &&
           hay->position.declination <= needle->position.declination &&
           hay_end.right_ascension >= needle_end.right_ascension && hay_end.declination >= needle_end.declination;
}

/// Checks if the 'hay' GlobeSection contains the 'needle' point
b8 globe_section_contains_point(GlobeSection* hay, GlobePosition* needle) {
    // clang-format off
    GlobePosition hay_end = globe_position_make(
        hay->position.right_ascension + hay->size.width,
        hay->position.declination + hay->size.height
    );
    // clang-format on
    return needle->right_ascension >= hay->position.right_ascension &&
           needle->right_ascension <= hay_end.right_ascension && needle->declination >= hay->position.declination &&
           needle->declination <= hay_end.declination;
}

/// Checks if the 'a' GlobeSection overlaps 'b'
b8 globe_section_overlap(GlobeSection* a, GlobeSection* b) {
    // Testing all the edges is definitely not the fastest approach, but it is rather easy to follow, which is a
    // requirement for solaris.
    // Here we will start out in the top left position
    GlobePosition point = a->position;
    if (globe_section_contains_point(b, &point)) {
        return true;
    }

    // Our top left edge doesn't fit, so we will test the top right edge
    point.right_ascension += a->size.width;
    if (globe_section_contains_point(b, &point)) {
        return true;
    }

    // Damn, now we have to go to our bottom right edge
    point.declination += a->size.height;
    if (globe_section_contains_point(b, &point)) {
        return true;
    }

    // Still no fit, our last contender is our bottom left edge
    point.right_ascension = a->position.right_ascension;
    return globe_section_contains_point(b, &point);
}

/// Creates a new GlobeNode inside the memory of the provided arena
GlobeNode* globe_node_make(MemoryArena* arena, GlobeSection* section, Object* object) {
    GlobeNode* node = (GlobeNode*) memory_arena_alloc(arena, sizeof(GlobeNode));
    node->previous = nil;
    node->next = nil;
    node->section = *section;
    node->object = object;
    return node;
}

/// Create an empty GlobeNode list
void globe_node_list_make(GlobeNodeList* list, MemoryArena* arena) {
    list->head = nil;
    list->tail = nil;
    list->size = 0;
    list->arena = arena;
}

/// Emplace a node into the list
void globe_node_list_emplace(GlobeNodeList* list, GlobeSection* section, Object* object) {
    GlobeNode* node = globe_node_make(list->arena, section, object);
    if (list->head == nil) {
        list->head = node;
        node->previous = nil;
        list->size++;
        return;
    }

    // run cursor to tail
    GlobeNode* it = list->head;
    for (; it->next != nil; it = it->next) { }

    it->next = node;
    node->previous = it;
    list->tail = node;
    list->size++;
}

/// Creates a new GlobeTree using the provided MemoryArena and section
GlobeTree* globe_tree_make(MemoryArena* arena, GlobeSection* section) {
    GlobeTree* tree = (GlobeTree*) memory_arena_alloc(arena, sizeof(GlobeTree));
    tree->arena = arena;
    tree->depth = 0;
    tree->section = *section;
    globe_node_list_make(&tree->nodes, arena);

    f64 width = tree->section.size.width / 2;
    f64 height = tree->section.size.height / 2;
    GlobeArea size = globe_area_make(width, height);

    for (usize i = 0; i < 2; ++i) {
        // clang-format off
        GlobePosition position_top = globe_position_make(
            tree->section.position.right_ascension + width * (f64) i,
            tree->section.position.declination
        );
        // clang-format on
        tree->sections[i] = globe_section_make(position_top, size);
        tree->trees[i] = nil;
    }

    for (usize i = 0; i < 2; ++i) {
        // clang-format off
        GlobePosition position_bottom = globe_position_make(
            tree->section.position.right_ascension + width * (f64) i,
            tree->section.position.declination + height
        );
        // clang-format on
        tree->sections[i + 2] = globe_section_make(position_bottom, size);
        tree->trees[i + 2] = nil;
    }
    return tree;
}

/// Calculates the section of an object
static GlobeSection object_calculate_section(Object* object) {
    f64 dimension = 1;
    if (object->dimension != 0) {
        dimension = object->dimension;
    }

    // convert arc seconds to degrees
    f64 angular_size = math_daa_to_degrees(0.0, 0.0, dimension);

    // clang-format off
    GlobePosition position = globe_position_make(
        object->position.right_ascension - 0.5 * angular_size,
        object->position.declination - 0.5 * angular_size
    );
    // clang-format on

    return globe_section_make(position, (GlobeArea){ angular_size, angular_size });
}

/// Creates the root GlobeTree using the provided MemoryArena, internally builds
/// the entire GlobeTree by emplacing all objects from the solaris catalog.
GlobeTree* globe_tree_make_root(MemoryArena* arena) {
    // clang-format off
    GlobeTree* root = globe_tree_make(arena, &(GlobeSection){
        globe_position_make(0.0, -90.0),
        globe_area_make(360.0, 180.0)
    });
    // clang-format on
    Catalog catalog = catalog_acquire();
    for (usize i = 0; i < catalog.object_count; ++i) {
        Object* object = catalog.objects + i;
        globe_tree_emplace(root, object);
    }
    return root;
}

/// Emplaces the specified object into the GlobeTree, using the object's
/// size to obtain the section
void globe_tree_emplace(GlobeTree* globe, Object* object) {
    GlobeSection section = object_calculate_section(object);

    // only if we aren't at max depth we are allowed to test for children
    if (globe->depth != SOLARIS_GLOBE_TREE_MAX_DEPTH) {
        // test children
        for (usize i = 0; i < 4; ++i) {
            GlobeSection* child_section = globe->sections + i;
            if (!globe_section_contains(child_section, &section)) {
                continue;
            }

            if (globe->trees[i] == nil) {
                globe->trees[i] = globe_tree_make(globe->arena, child_section);
                globe->trees[i]->depth = globe->depth + 1;
            }

            globe_tree_emplace(globe->trees[i], object);
            return;
        }
    }

    // check if object doesn't fit, should never happen!
    if (!globe_section_contains(&globe->section, &section)) {
        assert(false && "Object does not fit into GlobeTree!");
        return;
    }

    // whenever we land here, we are sure that the object didn't fit into any child trees
    globe_node_list_emplace(&globe->nodes, &section, object);
}

/// Determines the size (number of nodes) of the GlobeTree
usize globe_tree_size(GlobeTree* globe) {
    usize nodes = globe->nodes.size;
    for (usize i = 0; i < 4; ++i) {
        GlobeTree* child = globe->trees[i];
        if (child != nil) {
            nodes += globe_tree_size(child);
        }
    }
    return nodes;
}

/// Copies all the nodes from the GlobeTree to the specified list
static void globe_tree_copy_nodes_to_list(GlobeTree* globe, GlobeNodeList* list) {
    for (GlobeNode* it = globe->nodes.head; it != nil; it = it->next) {
        globe_node_list_emplace(list, &it->section, it->object);
    }

    // copy nodes from all children
    for (usize i = 0; i < 4; ++i) {
        GlobeTree* child = globe->trees[i];
        if (child) {
            globe_tree_copy_nodes_to_list(child, list);
        }
    }
}

/// Determines the GlobeNodes that are inside the searched section, adds them to the list
static void globe_tree_search_to_list(GlobeTree* globe, GlobeNodeList* list, GlobeSection* section) {
    // check nodes of the current GlobeTree and see if anything overlaps
    for (GlobeNode* it = globe->nodes.head; it != nil; it = it->next) {
        if (globe_section_overlap(section, &it->section)) {
            globe_node_list_emplace(list, &it->section, it->object);
        }
    }

    // check all children
    for (usize i = 0; i < 4; ++i) {
        GlobeTree* child = globe->trees[i];
        if (!child) {
            continue;
        }

        // if the searched section contains the child entirely, just copy all nodes
        if (globe_section_contains(section, &child->section)) {
            globe_tree_copy_nodes_to_list(child, list);
        } else {
            // when not, we need to go down the search path of the child
            if (globe_section_overlap(section, &child->section)) {
                globe_tree_search_to_list(child, list, section);
            }
        }
    }
}

/// Determines the GlobeNodes that are inside the searched section
GlobeNodeList globe_tree_search(GlobeTree* globe, MemoryArena* search_arena, GlobeSection* section) {
    GlobeNodeList result;
    globe_node_list_make(&result, search_arena);
    globe_tree_search_to_list(globe, &result, section);
    return result;
}
