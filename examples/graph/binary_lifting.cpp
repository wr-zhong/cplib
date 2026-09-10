// from CF 832D
// https://codeforces.com/problemset/problem/832/D
// AC: 1062ms

#include <graph/binary_lifting.hpp>

using edge = cp::graph::AdjEdge;

auto f = [](const int& a, const int& b) {
    return a + b;
};

using BL = cp::graph::BinaryLifting<int, decltype(f)>;

#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, q;
    cin >> n >> q;

    vector<vector<edge>> adj(n+1, vector<edge>());
    for (int i = 2; i <= n; i++) {
        int p;
        cin >> p;
        adj[p].push_back({i, 1});
        adj[i].push_back({p, 1});
    }

    BL bl(adj, 1, f, 0);
    
    for (int i = 0; i < q; i++) {
        int a, b, c;
        cin >> a >> b >> c;

        // 显然，更通用且正确的做法是算出三者相互的dist后 考虑所有p+q-r + 1

        // find dist between each pair
        auto p = bl.path_query(a, b);
        auto q = bl.path_query(b, c);
        auto r = bl.path_query(a, c);

        // then consider all choices
        int res = max({
            (p+q-r)/2,
            (p+r-q)/2,
            (q+r-p)/2
        });

        cout << res + 1 << "\n";
    }
}