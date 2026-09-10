#pragma once

#include <vector>
#include <algorithm>

namespace cp::graph {

// Kosaraju.
// Vertices must be 1-based.
class SCC {
public:
    // scc[i]: SCC id of vertex i (1-based)
    std::vector<int> scc;

    // `n` is the number of vertices.
    explicit SCC(int n)
    : n_(n),
      edges_(n + 1),
      edges_r_(n + 1),
      scc(n + 1),
      visited_(n + 1) {}

    void add_edge(int u, int v) {
        edges_[u].push_back(v);
        edges_r_[v].push_back(u);
    }

    // compute the id (0-based) of the scc each vertex is in.
    // returns the number of SCCs in the graph.
    int compute() {
        post_order_.clear();
        post_order_.reserve(n_);

        std::fill(visited_.begin(), visited_.end(), false);

        // vertices are 1-based
        for (int i = 1; i <= n_; ++i)
            if (!visited_[i]) dfs_(i);

        std::fill(visited_.begin(), visited_.end(), false);

        std::reverse(post_order_.begin(), post_order_.end());

        int cnt = 0;    // #sccs
        for (int u : post_order_)
            if (!visited_[u]) dfs_r_(u, cnt++);

        return cnt;
    }

private:
    int n_; // number of vertices

    std::vector<std::vector<int>> edges_;
    std::vector<std::vector<int>> edges_r_;

    std::vector<int> post_order_;
    std::vector<bool> visited_;

    void dfs_(int u) {
        visited_[u] = true;
        for (int v : edges_[u]) {
            if (!visited_[v]) dfs_(v);
        }
        post_order_.push_back(u);
    }

    void dfs_r_(int u, int id) {
        visited_[u] = true;
        scc[u] = id;
        for (int v : edges_r_[u]) {
            if (!visited_[v]) dfs_r_(v, id);
        }
    }
};

} // namespace cp::graph
