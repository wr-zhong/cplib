// CF 1971 H
// https://codeforces.com/problemset/problem/1971/H

// AC: 1656ms

#include <graph/twoSAT.hpp>

#include <bits/stdc++.h>
using namespace std;
using namespace cp::graph;

void solve() {
    int n;
    cin >> n;

    auto sat = TwoSAT(n);

    vector<int> r1(n);
    vector<int> r2(n);
    vector<int> r3(n);
    for (int i = 0; i < n; ++i) { cin >> r1[i]; }
    for (int i = 0; i < n; ++i) { cin >> r2[i]; }
    for (int i = 0; i < n; ++i) { cin >> r3[i]; }

    for (int i = 0; i < n; ++i) {
        // PITFALL: don't forget the minus sign in neg()
        int a = r1[i] > 0 ? r1[i] : sat.neg(-r1[i]);
        int b = r2[i] > 0 ? r2[i] : sat.neg(-r2[i]);
        int c = r3[i] > 0 ? r3[i] : sat.neg(-r3[i]);

        sat.add_or(a, b);
        sat.add_or(b, c);
        sat.add_or(a, c);
    }

    if (sat.is_satisfiable()) {
        cout << "YES\n";
    } else {
        cout << "NO\n";
    }
}

int main() {
    int T;
    cin >> T;
    while (T--) {
        solve();
    }
}