#pragma once

#include <vector>

namespace cp::range {

using std::vector;
using std::pair;

/**
 * typical usage: range tree on trees.
 * 
 * // example
 * TreeRange tr(...);
 * 
 * rt.update(tr.index(u), val);
 * 
 * auto [l, r] = tr.subtree_range(u);
 * rt.query(l, r);
 * OR
 * rt.query(tr.start[u], tr.end[u]);
 */
struct TreeRange {
    vector<int> start, end;

    /// @brief 
    /// @param children children[u] contains the id of all children of u.
    /// @param root id of the tree root
    explicit TreeRange(const vector<vector<int>>& children, int root)
        : start(children.size()), end(children.size()) {
        int timer = 0;

        auto dfs = [&](auto&& self, int u) -> void {
            start[u] = timer++;
            for (int v : children[u]) {
                self(self, v);
            }
            end[u] = timer;
        };

        dfs(dfs, root);
    }

    /// @brief return the index range of subtree rooted at `u`.
    /// @param u the id (origial index) of the tree node to query
    /// @return a pair [range_start, range_end].
    [[nodiscard]]
    pair<int, int> subtree_range(int u) const {
        return {start[u], end[u]};
    }

    /// @param u the id (origial index) of the tree node to query
    /// @return the flattened index of `u`.
    [[nodiscard]]
    int index(int u) const {
        return start[u];
    }
};

} // namespace cp::range
