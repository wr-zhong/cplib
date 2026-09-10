#include <graph/dijkstra.hpp>

namespace {
using AdjEdge = struct {
    int to;
    int colour;
};

/// @brief 4128 24t3 contest3-q2
/// @param adj adjlist
/// @param h elevation of each junction
/// @param k #bit in the set
/// @param start starting junction
/// @return dist[u][S]
decltype(auto) dijkstra(
    const std::vector<std::vector<AdjEdge>>& adj,
    const std::vector<int>& h,
    int k,
    int start
) {
    const int n = static_cast<int>(adj.size());

    std::vector<std::vector<long long>> dist(n, std::vector<long long>((1<<k), INF));
    std::vector<std::vector<bool>> seen(n, std::vector<bool>((1<<k), false));

    using State = std::tuple<long long, int, int>;    // [dist, vertex, S]
    std::priority_queue<State, std::vector<State>, std::greater<>> pq;

    dist[start][0] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {
        auto& [d, u, S] = pq.top();
        pq.pop();

        if (seen[u][S]) continue;

        dist[u][S] = d;
        seen[u][S] = true;

        for (const auto& [v, c] : adj[u]) {
            // emplace next state
            auto nxt_d = dist[u][S] + std::max(0, h[v]-h[u]);
            int nxt_S = S | (1<<(c-1));
            pq.emplace(nxt_d, v, nxt_S);
        }
    }

    return dist;
}

}