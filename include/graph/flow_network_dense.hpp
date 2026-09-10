#pragma once

#include <vector>
#include <queue>
#include <limits>

namespace {

constexpr long long INF = std::numeric_limits<long long>::max() / 8;

} // namespace

namespace cp::graph {

// Vertices are 1-indexed. Compatible with 0-indexed.
class [[deprecated("Use cp::graph::FlowNetwork (sparse adjacency-list Dinic) instead")]]
DenseFlowNetwork {
public:
    explicit DenseFlowNetwork(int n): n_(n) {
        adj_mat_.assign(n_ + 1, std::vector<long long>(n_ + 1));
        adj_list_.resize(n_ + 1);
        level_.resize(n_ + 1);
        up_to_child_.resize(n_ + 1);
    }

    // Adds a directed edge (u, v) with capacity `c`.
    // Parallel edges are merged.
    void add_edge(int u, int v, long long c) {
        adj_mat_[u][v] += c;
        adj_list_[u].push_back(v);
        adj_list_[v].push_back(u);
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

    // Returns the flow through edge u -> v (sum if parallel).
    // NOTE: This is correct only when the original graph does not
    // contain an edge v -> u. Otherwise adj_mat_[v][u] includes both
    // the original reverse capacity and the residual reverse capacity.
    long long get_flow_through(int u, int v) {
        return adj_mat_[v][u];
    }


private:
    // Sends `c` units of flow through (u, v).
    void send_flow(int u, int v, long long c) {
        adj_mat_[u][v] -= c;
        adj_mat_[v][u] += c;
    }

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

            for (int v : adj_list_[u]) {
                if (level_[v] == -1 && adj_mat_[u][v] > 0) {
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
        for (int& i = up_to_child_[u]; i < static_cast<int>(adj_list_[u].size()); i++) {
            // the child
            int v = adj_list_[u][i];

            // edge must be in the level graph with capacity remaining
            if (level_[v] == level_[u] + 1 && adj_mat_[u][v] > 0) {
                long long revised = augment(v, t, std::min(f, adj_mat_[u][v]));
                if (revised > 0) {
                    send_flow(u, v, revised);
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

private:
    // number of vertices
    int n_;

    // residual capacities (weights)
    std::vector<std::vector<long long>> adj_mat_;

    // adjacency list of the residual graph (only vertices)
    std::vector<std::vector<int>> adj_list_;

    // level_[v] stores dist from s to v
    std::vector<int> level_;

    // `up_to_child_[u]`: index of the first non-useless child in `adj_list_[u]`.
    // Augmenting starts on this child next time in the same level graph.
    std::vector<int> up_to_child_;
};

} // namespace cp::graph
