# CP Library

A personal C++ library of reusable data structures, algorithms, and utilities for competitive programming.

The library is designed primarily for contest use, with an emphasis on clear, readable implementations that are easy to copy and adapt for individual problems.

## Contents

### Range Queries

* Range tree with configurable merge operation
* Lazy range tree
* Sparse table
* 2D prefix sums
* Tree range utilities

### Graph

* BFS and DFS templates
* Dijkstra, Bellman-Ford and Floyd-Warshall
* MST (Kruskal)
* Strongly connected components and 2-SAT
* Topological sort and graph DP
* Articulation points and bridges
* Binary lifting (LCA)
* Flow network (Dinic) and utilities

### Math

* Modular arithmetic
* Combinations
* Prime testing and sieves
* Factorisation
* Coprimality utilities
* Square matrix and linear recurrence
* Exponentiation

### Geometry

* Generic points
* Cross products and orientation tests
* Polygon area
* Convex hull
* Lines and segments
* Half-plane
* Rotation
* Local coordinate systems

### Other Data Structures and Utilities

* Disjoint Set Union
* Discrete binary search
* Ternary search
* Coordinate compression

## Usage

The library is header-only. Components can be included directly, for example:

```cpp
#include <graph/dijkstra.hpp>
#include <range/range_tree.hpp>
#include <geom/convex_hull.hpp>
```

That said, the most common workflow is to copy the required components into a contest solution and remove anything that becomes unnecessary or duplicated after combining headers, such as repeated shared constants (e.g. `cp::geom::EPS`) and `#pragma once`.

## Examples

Selected usage examples are available under `examples/`.

Note: the examples directory is not intended to provide exhaustive coverage of every component.

To build the available examples, under the root directory:

```bash
cmake -B build
cmake --build build
```

## Notes

* `starter.cpp` is a personal CP setup.
* The library is designed primarily for contest use, with an emphasis on clear, readable implementations that are easy to copy and adapt for individual problems.
* C++20 features are used mainly in the geometry and math modules. Many other components use only C++17.
* Components are developed and refined through online judge submissions, contest use, and local experimentation.
