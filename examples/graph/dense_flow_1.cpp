/**
 * max flow
 * CF 546E
 * https://codeforces.com/problemset/problem/546/E
 * 
 * AC: 40ms
 */
#include <graph/flow_network_dense.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cp::graph;

/**
 * city: 1 .. n
 * city after roam: n+1 .. n+n
 * s: 2n+1
 * t: 2n+2
 */
int main() {
    int n, m;
    cin >> n >> m;

    int num_ver = 2*n + 2;
    const int s = 2*n+1, t = 2*n+2;

    auto bef = [](int i) { return i; };
    auto aft = [=](int i) { return i+n; };

    DenseFlowNetwork h(num_ver);

    // PITFALL: it is not necessary that \sum a_i == \sum b_i
    // check if \sum b_i is met as well!!!
    long long sum_a = 0, sum_b = 0;
    // s -> i
    for (int i = 1; i <= n; i++) {
        int a;
        cin >> a;
        sum_a += a;
        h.add_edge(s, bef(i), a);
    }

    // i' -> t
    for (int i = 1; i <= n; i++) {
        int b;
        cin >> b;
        sum_b += b;
        h.add_edge(aft(i), t, b);
    }

    // i -> i'
    for (int i = 1; i <= n; i++) {
        h.add_edge(bef(i), aft(i), INF);
    }

    // adj
    for (int i = 0; i < m; i++) {
        int p, q;
        cin >> p >> q;
        // NOTE: roads are bidirectional
        h.add_edge(bef(p), aft(q), INF);
        h.add_edge(bef(q), aft(p), INF);
    }

    auto max_f = h.max_flow(s, t);

    // cerr << "sum_a = " << sum_a << "\n";
    // cerr << "sum_b = " << sum_b << "\n";
    // cerr << "max_flow = " << max_f << "\n";

    // PITFALL: (max_f == sum_a == sum_b) You are comparing a boolean with sum_b!!!
    // Use logical AND!
    if (max_f == sum_a && sum_a == sum_b) {
        cout << "YES\n";
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cout << h.get_flow_through(bef(i), aft(j)) << " ";
            }
            cout << "\n";
        }
    } else {
        cout << "NO\n";
    }
}