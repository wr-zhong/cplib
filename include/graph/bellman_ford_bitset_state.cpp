#include "edges.hpp"

#include <limits>
#include <vector>

namespace {
/**
 * Bellman-Ford on state graph where state is represented by bitset
 * For copy & paste
 * When using, you should think about transition and state, and change them.
 * Current is from 4128 25t2 contest3 q2: Avatar Tour
 */
using Edge = cp::graph::Edge;

/// @brief SSSP, graph has negative edges but no negative cycles. Vertices are 1-based.
/// @param edges all edges {u, v, w}
/// @param p p[i]: nation of i. nations are 1-based.
/// @param n number of vertices (vertices are 1-based)
/// @param k number of bits in the set
/// @param start source vertex
/// @return dist[u][state]
[[nodiscard]]
inline std::vector<std::vector<long long>> bellman_ford(
    const std::vector<Edge>& edges,
    const std::vector<int>& p,
    int n,
    int k,
    int start
) {
    constexpr long long INF = std::numeric_limits<long long>::max() / 2;

    std::vector<std::vector<long long>> dist(n + 1, std::vector<long long>(1 << k, INF));

    // NOTE: start S is no 0!
    dist[start][1 << (p[start]-1)] = 0;

    // returns whether some edge is used to relax a distance
    auto relax = [&]() {
        bool relaxed = false;
        // for each edge (u, v), try to relax dist(s->v) using this edge
        // thus we would have relaxed each vertex v by checking all its incoming edges
        for (const auto& [u, v, w] : edges) {
            // for any S
            for (int S = 0; S < (1 << k); S++) {
                // we don't want to relax an edge from an unreachable vertex
                if (dist[u][S] == INF) continue;

                if (dist[v][S | (1 << (p[v]-1))] > dist[u][S] + w) {
                    dist[v][S | (1 << (p[v]-1))] = dist[u][S] + w;
                    relaxed = true;
                }
            }
        }

        return relaxed;
    };

    // NOTE: #global relaxations |V| - 1 has changed!!
    for (int i = 0; i < n * (1<<k) - 1; i++) {
        // if no updates are made in an entire round, we can early exit
        if (!relax()) {
            break;
        }
    }

    // a negative cycle exists iff any edge can be relaxed further
    // bool has_neg_cycle = relax(); // guaranteed no neg cycle

    return dist;
}

} // namespace