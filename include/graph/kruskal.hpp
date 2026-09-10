#pragma once

#include "edges.hpp"
#include "disjset/dsu.hpp"

#include <vector>
#include <algorithm>

namespace cp::graph {

/// @brief `tree` is the adjlist representation of the MST.
struct MST {
    long long weight;
    std::vector<std::vector<AdjEdge>> tree;
};

/// @param n number of vertices
/// @param edges a vector of `cp::graph::Edge`
/// @return a `{ weight, tree }`, where `tree` is the adjlist representation of the MST.
/// @note `edges` is passed by copy to avoid modifying the initial order. You might want
/// to change this to by reference if order doesn't matter.
[[nodiscard]]
inline MST kruskal(
    int n,
    std::vector<Edge> edges
) {
    auto dsu = cp::disjset::DSU(n + 1);

    std::sort(edges.begin(), edges.end());

    std::vector<std::vector<AdjEdge>> tree(n + 1);

    long long total_weight = 0;
    int edge_count = 0;

    for (const auto& e : edges) {
        if (dsu.find(e.u) == dsu.find(e.v)) continue;

        tree[e.u].push_back({e.v, e.w});
        tree[e.v].push_back({e.u, e.w});

        dsu.unite(e.u, e.v);

        total_weight += e.w;
        ++edge_count;

        // early stop
        if (edge_count == n - 1) break;
    }

    return {total_weight, std::move(tree)};
}

/// @brief `tree_edges` is an list of selected edges.
struct MST_edges {
    long long weight;
    std::vector<Edge> tree_edges;
};

/// @param n number of vertices
/// @param edges a vector of `cp::graph::Edge`
/// @return a `{ weight, tree_edges }`, where `tree_edges` is a collection of selected edges (edge list). 
/// @note `edges` is passed by copy to avoid modifying the initial order. You might want to change this
/// to by reference if order doesn't matter.
[[nodiscard]]
inline MST_edges kruskal_ret_edges(
    int n,
    std::vector<Edge> edges
) {
    auto dsu = cp::disjset::DSU(n + 1);

    std::sort(edges.begin(), edges.end());

    std::vector<Edge> tree_edges;   // start being empty

    long long total_weight = 0;
    int edge_count = 0;

    for (const auto& e : edges) {
        if (dsu.find(e.u) == dsu.find(e.v)) continue;

        tree_edges.push_back({e.u, e.v, e.w});

        dsu.unite(e.u, e.v);

        total_weight += e.w;
        ++edge_count;

        // early stop
        if (edge_count == n - 1) break;
    }

    return {total_weight, std::move(tree_edges)};
}

} // namespace cp::graph
