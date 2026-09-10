#pragma once

#include "edges.hpp" // struct Edge { int u, v, w; };

#include <limits>
#include <vector>

namespace cp::graph {

struct FloydWarshallResult {
    std::vector<std::vector<long long>> dist;
    std::vector<std::vector<int>> next;
    bool has_negative_cycle;
};

/// @brief restores the path from `s` to `t` after Floy-Warshall.
/// @param s from
/// @param t to
/// @param next the `next` matrix in `FloydWarshallResult`.
/// @return the path: s, ..., t
std::vector<int> floyd_restore_path(int s, int t, const std::vector<std::vector<int>>& next) {
    // no path
    if (next[s][t] == -1) return {};

    std::vector<int> path = {s};

    while (s != t) {
        s = next[s][t];
        path.push_back(s);
    }

    return path;
}

/// @brief All Pairs Shortest Distance (1-based vertices)
/// @param edges list of all *directed* edges {u->v, w}
/// @param n number of vertices (vertices are 1-based)
/// @return { 1-based dist[][], next[][], has_negative_cycle }
FloydWarshallResult floyd_warshall(
    const std::vector<Edge>& edges,
    int n
) {
    constexpr long long INF = std::numeric_limits<long long>::max() / 4;

    // INF-init
    std::vector<std::vector<long long>> dist(n + 1, std::vector<long long>(n + 1, INF));

    std::vector<std::vector<int>> next(n + 1, std::vector<int>(n + 1, -1));

    // every vertex can reach itself
    for (int i = 1; i <= n; ++i) {
        dist[i][i] = 0;
        next[i][i] = i;
    }

    // update dist for every edge
    for (const auto& [u, v, w] : edges) {
        // in case of multiple edges
        dist[u][v] = std::min(dist[u][v], static_cast<long long>(w));
        next[u][v] = v;
    }

    for (int k = 1; k <= n; ++k) {
        for (int u = 1; u <= n; ++u) {
            for (int v = 1; v <= n; ++v) {
                if (dist[u][k] + dist[k][v] < dist[u][v]) {
                    dist[u][v] = dist[u][k] + dist[k][v];
                    next[u][v] = next[u][k];        // update next
                }
            }
        }
    }

    bool has_negative_cycle = false;

    for (int i = 0; i < n; ++i) {
        if (dist[i][i] < 0) {
            has_negative_cycle = true;
            break;
        }
    }

    return {
        std::move(dist),
        std::move(next),
        has_negative_cycle
    };
}

} // namespace cp::graph