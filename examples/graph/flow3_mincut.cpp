/**
 * Problem E from
 * https://codeforces.com/gym/101982/attachments
 * AC: 31ms
 * 
 * A classic min cut problem where one vertex is splitted into two.
 * 
 * NOTE:
 * A -> B and C -> B, then barricading B (in cut_T) counts cost[B] twice!!
 * To count cost of each cell once, split it into _in and _out.
 * in -> out: cost
 * out -> another in: INF (never cut here)
 */
#include <graph/flow_network.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cp::graph;

// NOTE: grid is m by n!
int main() {
    int n, m, c;
    cin >> n >> m >> c;

    vector<vector<char>> grid(m, vector<char>(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> grid[i][j];
        }
    }

    vector<int> cost(c);
    for (int i = 0; i < c; i++) {
        cin >> cost[i];
    }

    // PITFALL: DOUBLE check if id is correct!! i should times #cols, which is n here!!
    auto id = [&](int i, int j) { return i * n + j; };

    auto in = [&](int i, int j) { return id(i, j); };

    auto out = [&](int i, int j) { return id(i, j) + n*m; };

    // construct Flow Network
    int S = -1;
    const int T = n*m*2;  // ver id are 0-based
    FlowNetwork h(n*m*2+1);

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};
    auto connect_adj = [&](int i, int j) {
        for (int k = 0; k < 4; k++) {
            int a = i+dx[k], b = j+dy[k];
            if (a >= 0 && a < m && b >= 0 && b < n) {
                // no need back to B
                if (grid[a][b] == 'B') {
                    continue;
                }

                h.add_edge(out(i, j), in(a, b), INF);
            }
        }
    };

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 'B') {
                S = out(i, j);
            }

            // u_in -> u_out
            if (grid[i][j] != 'B')  // PITFALL: B - a is invalid!
                h.add_edge(in(i, j), out(i, j), grid[i][j] == '.' ? INF : cost[grid[i][j] - 'a']);

            // boarder
            if (i == 0 || j == 0 || i == m-1 || j == n-1) {
                h.add_edge(out(i, j), T, INF);
            }

            connect_adj(i, j);
        }
    }

    auto min_cut = h.max_flow(S, T);

    cout << (min_cut < INF ? min_cut : -1); // note: < INF, not != INF
}