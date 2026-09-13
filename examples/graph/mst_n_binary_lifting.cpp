/**
 * CF 609E
 * https://codeforces.com/problemset/problem/609/E
 * AC: 1312ms
 * 
 * It's clear to see that we can't afford running a MST for each edge.
 * We need to solve the query for each edge quickly.
 * 
 * Can this be answered using the original MST?
 * Yes!
 * if adding edge {u, v} creates a cycle, we can remove the heaviest edge
 * in the original MST on the path from u to v. Then no cycle.
 * if the edge is already in the MST, nothing happens.
 * (moreover, heaviest edge on path u->v is {u, v}, so no special handles required)
 * 
 * How to get the max edge between u and v?
 * Use binary lifting. (LCA data structure)
 * where the precompt result should be max edge between u and its 2^k th parent.
 */

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