#pragma once

#include <vector>
#include <queue>
#include <limits>

namespace {

constexpr long long INF = std::numeric_limits<long long>::max() / 8;

} // namespace

namespace cp::graph {

// Vertices are 1-indexed. Compatible with 0-indexed.
class FlowNetwork {
public:
    explicit FlowNetwork(int n): n_(n) {
        adj_.resize(n_ + 1);
        level_.resize(n_ + 1);
        up_to_child_.resize(n_ + 1);
    }

    // Adds a directed edge (u, v) with capacity `c`.
    // Parallel edges are allowed.
    void add_edge(int u, int v, long long c) {
        Edge e{v, c, static_cast<int>(adj_[v].size()), true};
        Edge re{u, 0, static_cast<int>(adj_[u].size()), false};

        adj_[u].push_back(e);
        adj_[v].push_back(re);
    }

    // Returns the value of the max flow (ie, the capacity if the min cut).
    // Complexity:
    // O(min(V^2 E, Ef)). Almost never behaves anything close to the worst case.
    // Can prbably run O(V^2 E) = 1e12 in less than 1 second if the graph can
    // have any structure.
    // Runs in O(E sqrt(V)) on unit graph.
    [[nodiscard]]
    long long max_flow(int s, int t) {
        return dinic(s, t);
    }

    /// @brief Returns all min-cut edges of current max-flow.
    /// Useful when you are interested in the actual assignment.
    /// @param s sourse vertex
    /// @note Must be called after `max_flow`.
    std::vector<std::pair<int,int>> min_cut_edges(int s) {
        // std::vector<bool> vis(n_ + 1, false);
        // dfs_reach(s, vis);
        auto seen = min_cut_side(s);

        std::vector<std::pair<int,int>> ret;

        for (int u = 0; u <= n_; u++) { // fine: vis[0] is false if user passes 1-indexed vertices
            if (!seen[u]) continue;

            for (const auto& e : adj_[u]) {
                if (e.original && !seen[e.to]) {
                    ret.emplace_back(u, e.to);
                }
            }
        }

        return ret;
    }

    /// @brief Get vertices on the source side of the min cut.
    /// @param s source vertex
    /// @return A `std::vector<bool>` where `ret[i]` is true iff vertex `i` is in the S-side of the min cut.
    /// @note Must be called after max_flow().
    std::vector<bool> min_cut_side(int s) {
        // maybe, as a public interface, we should consider changing the return type to
        // vector<char> as proxy reference can break generic code / user might expect bool&
        std::vector<bool> seen(n_ + 1, false);
        dfs_reach(s, seen);
        return seen;
    }


private:
    // constructs the level graph and returns whether the sink is still reachable
    bool bfs(int s, int t) {
        std::fill(level_.begin(), level_.end(), -1);

        std::queue<int> q;
        q.push(s);
        level_[s] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            // reset next child
            // Note: index in adj_list_[u] is always 0-based.
            up_to_child_[u] = 0;

            for (const auto& e : adj_[u]) {
                int v = e.to;
                if (level_[v] == -1 && e.cap > 0) {
                    // unseen and still has capacity
                    level_[v] = level_[u] + 1;
                    q.push(v);
                }
            }
        }

        return level_[t] != -1;
    }

    // Finds an augmenting path from `u` to `t` with up to `f` units of flow.
    // Then sends the revised flow which saturates at least one edge.
    long long augment(int u, int t, long long f) {
        // base case: t reached, all in f can be sent.
        if (u == t) {
            return f;
        }

        // note the reference here! we increment up_to_child_[u], i.e. walk through u's neighbours
        // until we find a child that we can flow through
        for (int& i = up_to_child_[u]; i < static_cast<int>(adj_[u].size()); i++) {
            // the edge to that child
            auto& [v, cap, rev, dontcare] = adj_[u][i];   // reference is safe since augment doesn't push_back etc.

            // edge must be in the level graph with capacity remaining
            if (level_[v] == level_[u] + 1 && cap > 0) {
                long long revised = augment(v, t, std::min(f, cap));
                if (revised > 0) {
                    cap -= revised;
                    adj_[v][rev].cap += revised;
                    return revised;
                }
            }
        }

        // no child useable, never augment on u again
        level_[u] = -1;
        return 0;
    }

    // Returns the value of the max flow.
    long long dinic(int s, int t) {
        long long flow = 0;

        while (bfs(s, t)) {
            while (long long sent = augment(s, t, INF)) {
                // note: there is a possibility of overflow here
                // though this is unlikely to be tested in most problems.
                // If you do suspect about this, change here.
                flow += sent;
            }
        }

        return flow;
    }

    // checks reachability from u in the residual network
    // result is passed back in `vis`. `vis.size()` should be n_ + 1.
    void dfs_reach(int u, std::vector<bool>& seen) {
        seen[u] = true;

        for (const auto& e : adj_[u]) {
            if (e.cap > 0 && !seen[e.to]) {
                dfs_reach(e.to, seen);
            }
        }
    }

private:
    struct Edge {
        int to;             // destination vertex
        long long cap;      // capacity
        int rev;            // the index of its reversed edge in adj_[to]
        bool original;      // is this edge in the original graph (ie, not a reverse edge)
    };

    // number of vertices
    int n_;

    // residual capacities (weights)
    std::vector<std::vector<Edge>> adj_;

    // level_[v] stores dist from s to v
    std::vector<int> level_;

    // `up_to_child_[u]`: index of the first non-useless child's corresponding edge in `adj_[u]`.
    // Note in DFS we visit children in order in which they appear in adj_[u].
    // Augmenting starts on this child next time in the same level graph.
    std::vector<int> up_to_child_;
};

} // namespace cp::graph
