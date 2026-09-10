#include <range/lazy_range_tree.hpp>
#include <bits/stdc++.h>
using namespace std;
using cp::range::LazyRangeTree;

// actually has a easier prefix sum solution which is faster as well.
// "Card Trick": 1 <-> -1

int main() {
    int n, q;
    cin >> n >> q;

    vector<long long> v(n);
    for (int i = 0; i < n; i++) {
        cin >> v[i];
    }

    sort(v.rbegin(), v.rend());

    LazyRangeTree<int> rt(vector<int>(n, 0));

    // tree elements are initially 0;
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        l--;    // to index

        rt.add(l, r, 1);
    }

    vector<long long> scaler(n);
    for (int i = 0; i < n; i++) {
        scaler[i] = rt.at(i);
    }

    sort(scaler.rbegin(), scaler.rend());

    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += scaler[i] * v[i];
    }

    cout << sum << "\n";
}