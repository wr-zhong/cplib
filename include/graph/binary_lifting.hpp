#pragma once

#include "edges.hpp"    // struct AdjEdge { int to, w; };
#include <vector>

namespace cp::graph {

/**
 * Merge is a merge operation of type `T`. `T` is usally the type of weight.
 * Merge must be associative, ie, monoid: ((a, b), c) == (a, (b, c)).
 * identity_: (x, identity_) == x.
 * veritices are 1-based. (compatible with 0-based)
 * 
 * 'range tree' on tree edges. (LCA)
 */
template<
    class T,
    class Merge
>
class BinaryLifting {
public:
    // identity_: (x, identity_) == x.
    BinaryLifting(
        const std::vector<std::vector<AdjEdge>>& tree,
        int root,
        Merge merge = {},
        T identity = {}
    );

    [[nodiscard]]
    int lca(int u, int v) const;

    [[nodiscard]]
    T path_query(int u, int v) const;

private:
    int n_;
    int lg_;

    Merge merge_;
    T identity_;

    std::vector<int> depth_;

    // parent[u][k] is the 2^k-th parent of u.
    std::vector<std::vector<int>> parent_;

    // value_[v][k] is the merged result of path from u to parent[u][k].
    std::vector<std::vector<T>> value_;

    void dfs(
        int u,
        int parent,
        T AdjEdge_value,
        const std::vector<std::vector<AdjEdge>>& tree
    );

    void build();
};


template<class T, class Merge>
BinaryLifting<T, Merge>::BinaryLifting(
    const std::vector<std::vector<AdjEdge>>& tree,
    int root,
    Merge merge,
    T identity
)
: n_(tree.size() - 1)
, lg_(tree.empty() ? 0 : 32 - __builtin_clz(static_cast<unsigned>(n_)))   // since C++20: std::bit_width(static_cast<unsigned>(tree.size()))
, merge_(std::move(merge))
, identity_(identity)
, depth_(n_ + 1)
, parent_(n_ + 1, std::vector<int>(lg_, -1))
, value_(n_ + 1, std::vector<T>(lg_, identity))
{
    dfs(root, -1, identity, tree);
    build();
}

template<class T, class Merge>
void BinaryLifting<T, Merge>::dfs(
    int u,
    int p,
    T AdjEdge_value,
    const std::vector<std::vector<AdjEdge>>& tree
) {
    parent_[u][0] = p;
    value_[u][0] = AdjEdge_value;

    for (const auto [v, w] : tree[u]) {
        // replace visited by parent_ in a tree
        if (v == p) continue;

        depth_[v] = depth_[u] + 1;
        dfs(v, u, w, tree);
    }
}

template<class T, class Merge>
void BinaryLifting<T, Merge>::build() {
    // fill in the parent for each power of two up to n
    for (int k = 1; k < lg_; ++k) {
        for (int v = 1; v <= n_; ++v) {
            if (parent_[v][k-1] != -1) {
                // the 2^j-th parent is the 2^(j-1)-th parent of the 2^(j-1)-th parent
                parent_[v][k] = parent_[parent_[v][k-1]][k-1];
                value_[v][k] = merge_(
                    value_[v][k-1],
                    value_[parent_[v][k-1]][k-1]
                );
            }
        }
    }
}

template<class T, class Merge>
int BinaryLifting<T, Merge>::lca(int u, int v) const {
    // make sure u is deeper than v
    if (depth_[u] < depth_[v]) {
        std::swap(u, v);
    }

    // consider representing distance in binary. Each bit has to be either 0 (don't jump) or 1 (jump once)
    // repeatedly raise u by the largest possible power of two until it is the same depth as v
    for (int k = lg_ - 1; k >= 0; --k) {
        if (depth_[u] - (1 << k) >= depth_[v]) {
            u = parent_[u][k];
        }
    }

    if (u == v) {
        return u;
    }

    // Raise both nodes as much as possible without passing the LCA
    for (int k = lg_ - 1; k >= 0; --k) {
        if (parent_[u][k] != -1 && parent_[u][k] != parent_[v][k]) {
            u = parent_[u][k];
            v = parent_[v][k];
        }
    }
    
    // u and v are now distinct but have the same parent_, and that parent_ is the LCA
    return parent_[u][0];
}

template<class T, class Merge>
T BinaryLifting<T, Merge>::path_query(int u, int v) const {
    T ret = identity_;

    // make sure u is deeper than v
    if (depth_[u] < depth_[v]) {
        std::swap(u, v);
    }

    // consider representing distance in binary. Each bit has to be either 0 (don't jump) or 1 (jump once)
    // repeatedly raise u by the largest possible power of two until it is the same depth as v
    for (int k = lg_ - 1; k >= 0; --k) {
        if (depth_[u] - (1 << k) >= depth_[v]) {
            ret = merge_(ret, value_[u][k]);
            u = parent_[u][k];
        }
    }

    if (u == v) {
        return ret;
    }

    // Raise both nodes as much as possible without passing the LCA
    for (int k = lg_ - 1; k >= 0; --k) {
        if (parent_[u][k] != -1 && parent_[u][k] != parent_[v][k]) {
            ret = merge_(ret, value_[u][k]);
            ret = merge_(ret, value_[v][k]);

            u = parent_[u][k];
            v = parent_[v][k];
        }
    }
    
    // u and v are now distinct but have the same parent_, and that parent_ is the LCA
    ret = merge_(ret, value_[u][0]);
    ret = merge_(ret, value_[v][0]);
    return ret;
}

} // namespace cp::graph
