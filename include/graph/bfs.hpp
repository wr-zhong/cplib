#pragma once

#include <vector>
#include <queue>
#include <array>

namespace cp::graph {

/**
 * dist[i]: bfs distance from start to i.
 * NOTE: vertices not reachable have dist -1. Check before comparing!
 */
struct BFSResult {
    std::vector<int> dist;
    std::vector<int> pred;
};

/**
 * NOTE NOTE NOTE
 * dist is init to -1. CHECK == -1 for unreachable!!!!!!!!!!!
 */
[[nodiscard]]
inline BFSResult bfs(const std::vector<std::vector<int>>& adj, int start) {
    const int n = static_cast<int>(adj.size());

    std::vector<int> dist(n, -1);
    std::vector<int> pred(n, -1);

    std::queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();

        for (const int nxt : adj[cur]) {
            if (dist[nxt] != -1) {
                // seen
                continue;
            }

            dist[nxt] = dist[cur] + 1;
            pred[nxt] = cur;
            q.push(nxt);
        }
    }

    return {std::move(dist), std::move(pred)};
}


/**
 * Grid's 4-adjacency BFS
 */

// For the sake of copy & paste in contests, this is not put in a unnamed namespace
struct AlwaysTrue {
    constexpr bool operator()(int, int) const noexcept {
        return true;
    }
};

struct GridBFSResult {
    std::vector<std::vector<int>> dist;
    std::vector<std::vector<std::pair<int, int>>> pred;
};

/// @brief BFS on an n-by-m grid. All positions are 0-indexed.
/// @tparam CanVisit a predicate which returns whether (i, j) is visitable in the grid. Default to always true.
/// @param n number of rows
/// @param m number of columns
/// @param start {row, col} of the starting cell
/// @param can_visit a predicate which accepts `(int i, int j)`.
/// Note the grid's reference should be captured by this, thus not passed to grid_bfs.
/// @return { dist[i][j], pred[i][j] }
template <typename CanVisit = AlwaysTrue>
[[nodiscard]]
GridBFSResult grid_bfs(int n, int m, std::pair<int, int> start, CanVisit can_visit = {}) {
    std::vector<std::vector<int>> dist(n, std::vector<int>(m, -1));
    std::vector<std::vector<std::pair<int, int>>> pred(
        n,
        std::vector<std::pair<int, int>>(m, {-1, -1})
    );

    // {dr, dc}. Try adding an extra pair of braces to initialise DIR if this doesn't compile (Added)
    constexpr std::array<std::pair<int, int>, 4> DIRS{
        {
            {-1, 0},
            {1, 0},
            {0, -1},
            {0, 1},
        }
    };

    std::queue<std::pair<int, int>> q;

    auto [sr, sc] = start;
    dist[sr][sc] = 0;
    q.push(start);

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        for (const auto& [dr, dc] : DIRS) {
            int nr = r + dr;
            int nc = c + dc;

            if (nr < 0 || nr >= n || nc < 0 || nc >= m) {
                continue;
            }

            if (!can_visit(nr, nc)) {
                continue;
            }

            if (dist[nr][nc] != -1) {
                continue;
            }

            dist[nr][nc] = dist[r][c] + 1;
            pred[nr][nc] = {r, c};
            q.push({nr, nc});
        }
    }

    return {std::move(dist), std::move(pred)};
}

} // namespace cp::graph 