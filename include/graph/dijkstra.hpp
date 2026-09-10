#pragma once

#include "edges.hpp"    // struct AdjEdge { int to, w; };

#include <functional>
#include <queue>
#include <vector>
#include <set>
#include <limits>

namespace {

constexpr long long INF = std::numeric_limits<long long>::max() / 4;

} // namespace

namespace cp::graph {

struct DijkstraResult {
    std::vector<long long> dist;    // dist[i]: shortest distance from s -> i
    std::vector<int> pred;          // pred[i]: predecessor of i in the shortest path to i. -1 if i is unreachable or start.
};

/// @brief SSSP, no negative edges. Vertices can be 1-based or 0-based.
/// (just make sure the adjlist is correct).
/// @param adj the adjlist of `AdjEdge`
/// @param start the source vertex
/// @return { dist[], pred[] }
DijkstraResult dijkstra(
    const std::vector<std::vector<AdjEdge>>& adj,
    int start
) {
    const int n = static_cast<int>(adj.size());

    std::vector<long long> dist(n, INF);
    std::vector<int> pred(n, -1);
    std::vector<bool> known(n, false);

    using State = std::pair<long long, int>;    // [dist, vertex]
    std::priority_queue<State, std::vector<State>, std::greater<>> pq;

    dist[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {
        auto u = pq.top().second;
        pq.pop();

        if (known[u]) continue;

        known[u] = true;

        for (const auto& [v, w] : adj[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pred[v] = u;
                pq.emplace(dist[v], v);
            }
        }
    }

    return {std::move(dist), std::move(pred)};
}

/// @brief For each edge not in the Shortest Paths Tree, apply F. Graph must be a simple graph.
/// @tparam Directed whether the graph in `adj` is directed
/// @tparam F callback type
/// @param adj the adj list representation of graph
/// @param res `cp::graph::DijkstraResult` returned by dijkstra
/// @param f callback invoked with `f(u, v, edge)`
/// @note Only works for simple graphs (ie, no parallel edges).
template <bool Directed, typename F>
void for_each_non_SPT_edge(
    const std::vector<std::vector<AdjEdge>>& adj,
    const DijkstraResult& res,
    F&& f
) {
    int n = static_cast<int>(adj.size());

    for (int u = 0; u < n; ++u) {
        for (const auto& e : adj[u]) {
            int v = e.to;

            if constexpr (!Directed) {
                // avoid double counts for undigraph
                if (u > v) continue;

                if (res.pred[v] == u || res.pred[u] == v)
                    continue;
            } else {
                if (res.pred[v] == u)
                    continue;
            }

            f(u, v, e);
        }
    }
}

/// @brief Restores a shortest path from `s` to `t`.
/// @return a `std::vector<int>` containing SP {`s`, ..., `t`}.
std::vector<int> restore_SP(const DijkstraResult& res, int s, int t) {
    if (res.dist[t] == INF) return {};

    std::vector<int> path;

    for (int cur = t; cur != s; cur = res.pred[cur]) {
        path.push_back(cur);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

/// @brief Apply f to every edge not in the restored shortest path from s to t.
/// @tparam F Callback type.
/// @tparam Directed Whether the graph is directed.
/// @param adj Graph adjacency list.
/// @param res Dijkstra result containing predecessor information.
/// @param s Source vertex.
/// @param t Target vertex.
/// @param f Callback invoked with (u, v, edge).
template <bool Directed, typename F>
void for_each_edge_not_in_restored_SP(
    const std::vector<std::vector<AdjEdge>>& adj,
    const DijkstraResult& res,
    int s,
    int t,
    F&& f
) {
    using EdgeKey = std::pair<int, int>;

    // change this to std::unordered_set if you want a good looking time complexity
    // set is quick enough for contests
    std::set<EdgeKey> sp_edges;

    // restore SP and record its edges
    for (int cur = t; cur != s; cur = res.pred[cur]) {
        int p = res.pred[cur];

        if constexpr (Directed) {
            sp_edges.emplace(p, cur);
        } else {
            // record (u, v) in ascending order, ie, u <= v
            sp_edges.emplace(std::min(p, cur),
                             std::max(p, cur));
        }
    }

    int n = static_cast<int>(adj.size());

    for (int u = 0; u < n; ++u) {
        for (const auto& e : adj[u]) {
            int v = e.to;

            if constexpr (!Directed) {
                if (u > v) continue;
            }

            bool on_sp;

            if constexpr (Directed) {
                on_sp = sp_edges.contains({u, v});
            } else {
                on_sp = sp_edges.contains(
                    {std::min(u, v), std::max(u, v)}
                );
            }

            if (!on_sp) { f(u, v, e); }
        }
    }
}


/// @brief Apply f to every edge that does not belong to any shortest path from s to t.
/// @tparam F Callback type.
/// @tparam Directed Whether the graph is directed.
/// @param adj Graph adjacency list.
/// @param from_s Dijkstra result from s.
/// @param from_t Dijkstra result from t on the reversed graph.
/// @param s Source vertex.
/// @param t Target vertex.
/// @param f Callback invoked with `(u, v, edge)`.
template <bool Directed, typename F>
void for_each_edge_not_in_any_SP(
    const std::vector<std::vector<AdjEdge>>& adj,
    const DijkstraResult& from_s,
    const DijkstraResult& from_t,
    int t,
    F&& f
) {
    const auto shortest = from_s.dist[t];
    int n = static_cast<int>(adj.size());

    for (int u = 0; u < n; ++u) {
        for (const auto& e : adj[u]) {
            int v = e.to;

            if constexpr (!Directed) {
                // avoid double counts
                // actually doesn't matter much here
                if (u > v) continue;
            }

            bool on_sp = false;

            if (from_s.dist[u] != INF && from_t.dist[v] != INF) {
                on_sp |= (from_s.dist[u] + e.w + from_t.dist[v] == shortest);
            }

            if constexpr (!Directed) {
                // check using {u, v} as (v, u)
                if (from_s.dist[v] != INF && from_t.dist[u] != INF) {
                    on_sp |= (from_s.dist[v] + e.w + from_t.dist[u] == shortest);
                }
            }

            if (!on_sp) { f(u, v, e); }
        }
    }
}

} // namespace cp::graph
