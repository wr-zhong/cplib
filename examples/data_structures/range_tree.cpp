// This problem is from
// https://www.codechef.com/problems/ACM14KN4

// range tree (point update, range query)

#include <bits/stdc++.h>
#include <range/range_tree.hpp>

using namespace std;
using cp::range::RangeTree;

struct Node {
    long long max;
    long long sum;
    long long first;
    long long last;
    bool inc;
    bool dec;
};

auto f_merge = [](const Node &a, const Node &b) -> Node {
    return {
        max(a.max, b.max),
        a.sum + b.sum,
        a.first,
        b.last,
        a.last <= b.first && a.inc && b.inc,
        a.last >= b.first && a.dec && b.dec
    };
};


int main() {
    int n, q;
    cin >> n >> q;

    vector<Node> nodes(n);  // 0-indexed

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        nodes[i] = {x, x, x, x, true, true};
    }

    auto rt = RangeTree<Node, decltype(f_merge)>(nodes, f_merge);

    for (int i = 0; i < q; i++) {
        char op;
        cin >> op;

        if (op == 'U') {
            int x;
            long long d;
            cin >> x >> d;
            
            rt.update(x - 1, {d, d, d, d, true, true});
        } else {
            int x, y;
            cin >> x >> y;
            x--;    // to index

            auto res = rt.query(x, y);

            if (op == 'M') cout << res.max << "\n";
            if (op == 'S') cout << res.sum << "\n";
            if (op == 'I') cout << (res.inc ? 1 : 0) << "\n";
            if (op == 'D') cout << (res.dec ? 1 : 0) << "\n";
        }
    }
}