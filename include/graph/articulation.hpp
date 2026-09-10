#pragma once

#include <vector>
#include <utility>

////////////////////////////////////////////////////////////////////
// For copy & paste
namespace {

using std::vector;
using std::pair;

int n;  // number of vertices

/**
 * Bridges
 * 
 * for copy & paste
 * note: init num to -1 as unseen!
 */

int timer = 0;
vector<int> num(n + 1, -1), low(n + 1);
vector<pair<int, int>> bridges;

auto dfs = [&](auto&& self, int u, int p) -> void {
    num[u] = low[u] = timer++;

    for (int v : adj[u]) {
        if (v == p) continue;

        if (num[v] == -1) {
            self(self, v, u);

            low[u] = min(low[u], low[v]);

            if (low[v] > num[u]) {
                bridges.push_back({u, v});
            }
        } else {
            low[u] = min(low[u], num[v]);
        }
    }
};

/* <Usage>

for (int i = 1; i <= n; i++)
    if (num[i] == -1)
        dfs(dfs, i, -1);

*/


/**
 * Ariticulation Points
 * 
 * copy & paste
 * note: init num to -1 as unseen!
 */

int timer = 0;
vector<int> num(n + 1, -1), low(n + 1);
vector<bool> is_art(n + 1);

auto dfs = [&](auto&& self, int u, int p) -> void {
    num[u] = low[u] = timer++;

    int child = 0;

    for (int v : adj[u]) {
        if (v == p) continue;

        if (num[v] == -1) {
            child++;

            self(self, v, u);

            low[u] = min(low[u], low[v]);

            if (p != -1 && low[v] >= num[u])
                is_art[u] = true;
        } else {
            low[u] = min(low[u], num[v]);
        }
    }

    if (p == -1 && child >= 2)
        is_art[u] = true;
};

/* <Usage>

for (int i = 1; i <= n; i++)
    if (num[i] == -1)
        dfs(dfs, i, -1);

*/

} // namespace

////////////////////////////////////////////////////////////////////

namespace cp::graph {

struct ArticulationResult {
    std::vector<int> num;
    std::vector<int> low;

    std::vector<std::pair<int, int>> bridges;
    std::vector<int> articulation_points;
};

/// @brief Directed unweighted simple graph. Default to 1-based.
///
/// Note: Caller must pass the parent of root as -1.
/// @param graph the adj list
/// @param index_base 0/1. default to 1
/// @return `{ num, low, bridges, articulation_points }`.
[[nodiscard]]
inline ArticulationResult articulation(const std::vector<std::vector<int>>& graph, int index_base = 1) {

    int n = static_cast<int>(graph.size());

    ArticulationResult res;
    res.num.assign(n, -1);
    res.low.resize(n);

    std::vector<bool> is_art(n);

    int timer = 0;

    auto dfs = [&](auto&& self, int u, int parent) -> void {
        res.num[u] = res.low[u] = timer++;

        int child_cnt = 0;

        for (int v : graph[u]) if (v != parent) {
            if (res.num[v] == -1) {
                // unseen
                ++child_cnt;

                self(self, v, u);

                res.low[u] = std::min(res.low[u], res.low[v]);
                // bridge
                if (res.low[v] > res.num[u]) {
                    res.bridges.emplace_back(u, v);
                }

                // articulation point (non-root)
                if (parent != -1 && res.low[v] >= res.num[u]) {
                    is_art[u] = true;
                }

            } else {
                res.low[u] = std::min(res.low[u], res.num[v]);
            }
        }

        if (parent == -1 && child_cnt >= 2) {
            is_art[u] = true;
        }
    };

    for (int i = index_base; i < n; ++i) {
        if (res.num[i] == -1) {
            dfs(dfs, i, -1);
        }
    }

    for (int i = index_base; i < n; ++i) {
        if (is_art[i]) {
            res.articulation_points.push_back(i);
        }
    }

    return res;
}

} // namespace cp::graph
