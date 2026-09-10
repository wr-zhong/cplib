#pragma once

#include <vector>
#include <algorithm>

namespace cp::graph {

/// @param adj the adjlist of a DAG.
/// @param index_base 0/1-indexed. Default to 1-indexed
/// @return the vertices in topological order
[[nodiscard]]
std::vector<int> topsort(const std::vector<std::vector<int>>& adj, int index_base = 1) {
    // the reversed dfs postorder on a DAG is in topological order
    const int n = static_cast<int>(adj.size());

    std::vector<bool> seen(n);
    std::vector<int> postorder;
    postorder.reserve(n);

    auto dfs = [&](auto&& self, int u) -> void {
        seen[u] = true;
        for (int v : adj[u]) {
            if (!seen[v]) {
                self(self, v);
            }
        }
        postorder.push_back(u);  // postorder
    };

    for (int u = index_base; u < n; ++u) {
        if (!seen[u]) {
            dfs(dfs, u);
        }
    }

    std::reverse(postorder.begin(), postorder.end());
    return postorder;
}

/// @brief Performs DP on a DAG.
/// @tparam Transition the recurrence relation of DP. Note `dp[]` should be captured by this.
/// @param adj the adj list representation of the DAG
/// @param relax for any `u` such that `u -> v`, `relax(v, u)` tries to relax `dp[v]` using `dp[u]`.
/// @param index_base 0/1-indexed. Default to 0-indexed
template <class Transition>
void dag_dp(const std::vector<std::vector<int>>& adj, Transition relax, int index_base = 0) {
    const auto order = topological_sort(adj, index_base);

    // process u in topological order
    for (int u : order) {
        // for each v s.t. u -> v
        for (int v : adj[u]) {
            // relax dp(v)
            relax(v, u);
        }
    }
}

} // namespace cp::graph
