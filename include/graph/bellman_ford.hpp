#pragma once

#include "edges.hpp"

#include <limits>
#include <vector>

namespace cp::graph {

struct BellmanFordResult {
    bool has_negative_cycle;
    std::vector<long long> dist;
    std::vector<int> pred;
};

/// @brief SSSP, graph has negative edges but no negative cycles. Vertices are 1-based.
/// @param edges all edges {u, v, w}
/// @param n number of vertices (vertices are 1-based)
/// @param start source vertex
/// @return { has_negative_cycle, dist[], pred[] }
[[nodiscard]]
inline BellmanFordResult bellman_ford(
    const std::vector<Edge>& edges,
    int n,
    int start
) {
    constexpr long long INF = std::numeric_limits<long long>::max() / 2;

    std::vector<long long> dist(n + 1, INF);
    std::vector<int> pred(n + 1, -1);

    dist[start] = 0;

    // returns whether some edge is used to relax a distance
    auto relax = [&]() {
        bool relaxed = false;
        // for each edge (u, v), try to relax dist(s->v) using this edge
        // thus we would have relaxed each vertex v by checking all its incoming edges
        for (const auto& [u, v, w] : edges) {
            // we don't want to relax an edge from an unreachable vertex
            if (dist[u] == INF) continue;

            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                pred[v] = u;
                relaxed = true;
            }
        }

        return relaxed;
    };

    // V - 1 global relaxations
    for (int i = 0; i < n - 1; ++i) {
        // if no updates are made in an entire round, we can early exit
        if (!relax()) {
            break;
        }
    }

    // a negative cycle exists iff any edge can be relaxed further
    bool has_neg_cycle = relax();

    return {
        has_neg_cycle,
        std::move(dist),
        std::move(pred)
    };
}

/// @brief Works with both 0-based & 1-based vertices.
/// @param edges edge list
/// @param n number of vertices
/// @return if the graph has a negative cycle.
[[nodiscard]]
bool has_negative_cycle(const std::vector<Edge>& edges, int n) {
    // ensure all reachable
    std::vector<long long> dist(n + 1, 0);  // +1 for 1-based vertices

    for (int i = 0; i < n; ++i) {
        bool relaxed = false;

        for (const auto& e : edges) {
            if (dist[e.v] > dist[e.u] + e.w) {
                dist[e.v] = dist[e.u] + e.w;
                relaxed = true;

                if (i == n - 1) {
                    return true;
                }
            }
        }

        if (!relaxed) {
            return false;
        }
    }

    return false;
}

} // namespace cp::graph
