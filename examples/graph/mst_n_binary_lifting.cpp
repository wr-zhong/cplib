// CF 609E
// AC: 1312ms

#include <graph/kruskal.hpp>
#include <graph/binary_lifting.hpp>


#include <bits/stdc++.h>
using namespace std;
using namespace cp::graph;

int main() {
    int n, m;
    cin >> n >> m;

    vector<::cp::graph::Edge> edges(m);

    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    auto mst = kruskal(n, edges);
    // now we have the original mst

    auto f = [](const int a, const int b) { return max(a, b); };
    auto bl = BinaryLifting<int, decltype(f)>(mst.tree, 1, f);

    for (int i = 0; i < m; i++) {
        int mx = bl.path_query(edges[i].u, edges[i].v);
        cout << mst.weight - mx + edges[i].w << "\n";
    }
}