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

#ifndef SOLARIS_GLOBE_H
#define SOLARIS_GLOBE_H

#include <solaris/linear.h>
#include <solaris/object.h>

#ifndef SOLARIS_GLOBE_TREE_MAX_DEPTH
#define SOLARIS_GLOBE_TREE_MAX_DEPTH 8
#endif// SOLARIS_GLOBE_TREE_MAX_DEPTH

/// GlobeArea Represents an area in the globe
typedef struct GlobeArea {
    f64 width;
    f64 height;
} GlobeArea;

/// Creates a new GlobeArea
/// @param width The width
/// @param height The height
/// @return A new GlobeArea
SOLARIS_API GlobeArea globe_area_make(f64 width, f64 height);

/// GlobePosition Represents a position in the globe
typedef struct GlobePosition {
    f64 right_ascension;
    f64 declination;
} GlobePosition;

/// Create a new GlobePosition
/// @param right_ascension The right ascension
/// @param declination The declination
/// @return A new GlobePosition
SOLARIS_API GlobePosition globe_position_make(f64 right_ascension, f64 declination);

/// GlobeSection Represents a section in the globe
typedef struct GlobeSection {
    GlobePosition position;
    GlobeArea size;
} GlobeSection;

/// Create a new GlobeSection
/// @param position The position in the globe
/// @param size The angular size of the section
/// @return A new GlobeSection
SOLARIS_API GlobeSection globe_section_make(GlobePosition position, GlobeArea size);

/// Checks if the 'hay' GlobeSection contains the 'needle'
/// @param hay Containing GlobeSection
/// @param needle The needle in the hay;)
/// @return Boolean that indicates whether the 'hay' GlobeSection contains the 'needle'
SOLARIS_API b8 globe_section_contains(GlobeSection* hay, GlobeSection* needle);

/// Checks if the 'hay' GlobeSection contains the 'needle' point
/// @param hay Containing GlobeSection
/// @param needle The needle in the hay which is the search point
/// @return Boolean that indicates whether the 'hay' GlobeSection contains the 'needle'
SOLARIS_API b8 globe_section_contains_point(GlobeSection* hay, GlobePosition* needle);

/// Checks if the 'a' GlobeSection overlaps 'b'
/// @param a One GlobeSection
/// @param b Other GlobeSection
/// @return Boolean that indicates whether the two GlobeSections overlap
SOLARIS_API b8 globe_section_overlap(GlobeSection* a, GlobeSection* b);

/// Forward declare GlobeNode
typedef struct GlobeNode GlobeNode;

/// A GlobeNode binds an actual object to a section
typedef struct GlobeNode {
    /// The previous GlobeNode in the list
    GlobeNode* previous;

    /// The next GlobeNode in the list
    GlobeNode* next;

    /// The section of the this GlobeNode
    GlobeSection section;

    /// The object which is associated with this GlobeNode.
    /// It lives inside the generated catalog, thereby being readonly.
    Object* object;
} GlobeNode;

/// Creates a new GlobeNode inside the memory of the provided arena
/// @param arena The arena where the node lives in
/// @param section The section of the object
/// @param object The object itself
/// @return A new GlobeNode
GlobeNode* globe_node_make(MemoryArena* arena, GlobeSection* section, Object* object);

typedef struct GlobeNodeList {
    /// The head of the list, may be nil if the list is empty
    GlobeNode* head;

    /// The tail of the list, may be nil if the list is empty
    GlobeNode* tail;

    /// The number of nodes in the list
    usize size;

    /// The arena where the node allocations are placed in
    MemoryArena* arena;
} GlobeNodeList;

/// Create an empty GlobeNode list
/// @param list The actual list
/// @param arena The arena where the allocations lie in
void globe_node_list_make(GlobeNodeList* list, MemoryArena* arena);

/// Emplace a node into the list
/// @param list The list where the node is placed in
/// @param section The section of the object
/// @param object The object itself
void globe_node_list_emplace(GlobeNodeList* list, GlobeSection* section, Object* object);

/// Forward declare GlobeTree
typedef struct GlobeTree GlobeTree;

/// GlobeTree is a spatial acceleration structure for storing fixed objects.
/// it enables fast querying for
/// <ul>
///     <li>entire sections in the tree
///     <li>specific positions
///     <li>individual objects
/// </ul>
typedef struct GlobeTree {
    /// The potential children which live inside the arena
    /// A nil GlobeTree indicates that it has no more children
    /// in the accessed part
    GlobeTree* trees[4];

    /// The child sections of this GlobeTree, which are guaranteed
    /// to have the same size
    GlobeSection sections[4];

    /// The section of this GlobeTree
    GlobeSection section;

    /// The actual nodes of the GlobeTree
    GlobeNodeList nodes;

    /// The depth of this GlobeTree starting from the root. This gives a good
    /// indication if we have too deep trees.
    usize depth;

    /// A pointer to the arena which is used for all allocations.
    /// That includes the child GlobeTrees, as well as all of the
    /// GlobeNodes.
    MemoryArena* arena;
} GlobeTree;

/// Creates a new GlobeTree using the provided MemoryArena and section
/// @param arena The arena where all objects of the GlobeTree live in, including the GlobeTree itself
/// @param section The section which the created GlobeTree covers
/// @return A new GlobeTree
GlobeTree* globe_tree_make(MemoryArena* arena, GlobeSection* section);

/// Creates the root GlobeTree using the provided MemoryArena, internally builds
/// the entire GlobeTree by emplacing all objects from the solaris catalog.
/// @param arena The arena where all objects of the GlobeTree live in, including the GlobeTree itself
/// @return A new root GlobeTree
GlobeTree* globe_tree_make_root(MemoryArena* arena);

/// Emplaces the specified object into the GlobeTree, using the object's
/// size to obtain the section
/// @param globe The GlobeTree where the object is placed in
/// @param object The actual object
void globe_tree_emplace(GlobeTree* globe, Object* object);

/// Determines the size (number of nodes) of the GlobeTree
/// @return Number of nodes in the GlobeTree
usize globe_tree_size(GlobeTree* globe);

/// Determines the GlobeNodes that are inside the searched section
/// @param globe The GlobeTree which is searched
/// @param search_arena The arena where the GlobeNodeList entries will be allocated in
/// @param section The section for the search
/// @return A new GlobeNodeList whose nodes live inside the search_arena, contains all nodes
//          that are inside the searched section
GlobeNodeList globe_tree_search(GlobeTree* globe, MemoryArena* search_arena, GlobeSection* section);

#endif// SOLARIS_GLOBE_H
