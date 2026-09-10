/**
 * From: CF 1343E
 * https://codeforces.com/problemset/problem/1343/E
 * 
 * on the unweighted graph, use bfs to compute the shortest distance
 * starting from a, b, c. 
 * 
 * Then for each vertex k, use it as an intermediate:
 * a ->( k -> b -> k) -> c. Thus
 * #edges used once = dist_a[k] + dist_c[k];
 * #edges used twice = dist_b[k].
 * 
 * Note, `once` can be over estimated, but if so
 * the one that is not over estimated would also be considered
 * since we consider all k.
 * NOTE: discard over-estimated cases when accessing ps! Otherwise may out of bound
 * 
 */
#include <graph/bfs.hpp>
#include <bits/stdc++.h>
using namespace std;
using namespace cp::graph;

struct Len {
    int once;
    int twice;
};

// O(n+m)
void solve() {
    int n, m, a, b, c;
    cin >> n >> m >> a >> b >> c;

    vector<int> p;
    p.reserve(m);
    for (int i = 0; i < m; i++) {
        int x;
        cin >> x;
        p.push_back(x);
    }

    vector<vector<int>> adj(n+1, vector<int>());
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // dist_a[k]: shortest dist from a to k
    auto dist_a = bfs(adj, a).dist;
    auto dist_b = bfs(adj, b).dist;
    auto dist_c = bfs(adj, c).dist;

    sort(p.begin(), p.end());
    // prefix sums
    vector<long long> ps(m+1, 0);
    for (int i = 1; i <= m; i++) {
        ps[i] = ps[i-1] + p[i-1];
    }

    long long ans = LLONG_MAX / 2;

    for (int k = 1; k <= n; k++) {
        int once = dist_a[k] + dist_c[k];
        int twice = dist_b[k];
        // note, `once` can be over estimated, but since we consider all k,
        // the one that is not over estimated would also be considered, so
        // the ans is not affected.
        // NOTE: discard such cases when accessing ps! otherwise may out of bound
        // cerr << "k=" << k << ", once=" << once << ", twice=" << twice << "\n";
        // 
        // out of bound since once is over estimated
        if (twice + once > m) continue;

        auto price = ps[twice] + ps[once + twice];
        // cerr << "price=" << price << "\n";
        ans = min(ans, price);
    }

    cout << ans << "\n";
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        solve();
    }
}