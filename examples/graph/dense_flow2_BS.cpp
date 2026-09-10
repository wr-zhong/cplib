/**
 * Flow Network & Binary Search (& Floyd-Warshall)
 * 
 * https://codeforces.com/problemset/problem/852/D
 * AC: 546ms ([[deprecated]] cp::graph::DenseFlowNetwork)
 * AC: 453ms (cp::graph::FlowNetwork)
 * 
 * 
 * if I known which cities I can get to from a city u
 * within time t, I can use these edges to build a flow
 * network:
 * S -> u: num Teams starting here,
 * u -> v': INF, where v is reachable from u in time t
 * v' -> T : 1, if this city is reached by some team
 * 
 * then check if f(s, t) >= K
 * 
 * We can run Floyd for All pairs O(V^3)    -> 2.16e8, quite concerning
 * (alternatively: run a bfs for each time starting at a city)
 * Then Binary Search for each flow network: log(max_path_len) * V^2 * E
 * 
 * 
 * IMPORTANT:
 * if problem has vars like V, N, etc., be EXTRA careful about which you should use
 * eg, vertex num = ? input num = ?
 * 
 * Be careful when adding edges to H. S->i and to(i) -> T!
 * Can write an identiity function from(i). Reduces mistake where from(i) should be used.
 * 
 * Also, always consider if the LOWER BOUND of BS can be 0!
 */

#include <graph/flow_network.hpp>
#include <graph/floyd_warshall.hpp>
#include <util/binary_search.hpp>

#include <bits/stdc++.h>
using namespace std;
using namespace cp::graph;
using namespace cp::util;

int main() {
    int V, E, N, K;
    cin >> V >> E >> N >> K;

    // num teams starting at city i
    vector<int> num_teams(V+1);
    // PITFALL: next line contains N integers!!!!! Not V
    for (int i = 0; i < N; i++) {
        int c;
        cin >> c;
        num_teams[c]++;
    }

    vector<Edge> edges;
    for (int i = 0; i < E; i++) {
        int a, b, t;
        cin >> a >> b >> t;
        edges.push_back({a, b, t});
        edges.push_back({b, a, t});
    }

    auto floyd_res = floyd_warshall(edges, V);
    auto dist = floyd_res.dist;

    long long max_len = 0;
    for (int i = 1; i <= V; i++) {
        for (int j = 1; j <= V; j++) {
            max_len = max(max_len, dist[i][j]);
        }
    }

    auto to = [=](int i) { return i+V; };
    const int S = 2*V+1;
    const int T = 2*V+2;

    auto check = [&](int time) {
        /**
         * u: 1 .. V
         * u': V+1 .. V+V
         * S: 2V+1
         * T: 2V+2
         */
        FlowNetwork H(2*V + 2);
        for (int i = 1; i <= V; i++) {
            H.add_edge(S, i, num_teams[i]);
            H.add_edge(to(i), T, 1);
        }

        for (int i = 1; i <= V; i++) {
            for (int j = 1; j <= V; j++) {
                if (dist[i][j] <= time) {
                    H.add_edge(i, to(j), INF);
                }
            }
        }

        auto maxf = H.max_flow(S, T);
        // cerr << "time is " << time << " max_flow is " << maxf << "\n";;

        return maxf >= K;
    };

    if (check(max_len) == false) {
        cout << -1;
    } else {
        // PITFALL: time (lo) can be 0!!
        auto minT = first_true(0, max_len, check);
        cout << minT;
    }
}