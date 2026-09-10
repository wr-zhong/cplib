#include <vector>

namespace detail {
using std::vector;
int n;
vector<vector<int>> adj, adj_r;

/////////////////////////////////////////////////

/**
 * dfs
 */

vector<bool> seen(n+1);

auto dfs = [&](auto&& self, int u) -> void {
    seen[u] = true;

    for (int v : adj[u]) {
        if (!seen[v]) {
            self(self, v);
        }
    }
};

/**
 * dfs_r
 */

vector<bool> seen_r(n+1);

auto dfs_r = [&](auto&& self, int u) -> void {
    seen_r[u] = true;

    for (int v : adj_r[u]) {
        if (!seen_r[v]) {
            self(self, v);
        }
    }
};

/**
 * dfs cycle check on digraph
 */

vector<bool> seen_c(n+1);
vector<bool> onstack(n+1);

auto dfs_has_cycle = [&](auto&& self, int u) -> bool {
    seen_c[u] = true;
    onstack[u] = true;

    for (int v : adj[u]) {
        if (onstack[v]) {
            return true;
        }
        if (!seen_c[v] && self(self, v)) {
            return true;
        }
    }

    onstack[u] = false;
    return false;
};

// Check on all vertices. Double check the index if using this.
// for (int u = 1; u <= n; ++u) {
//     if (!seen_c[u] && dfs_has_cycle(dfs_has_cycle, u)) {
//         return true;
//     }
// }

/////////////////////////////////////////////////
} // namespace detail
