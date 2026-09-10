// CF 1263D
// Secret Password
// https://codeforces.com/problemset/problem/1263/D

#include <disjset/dsu.hpp>
#include <bits/stdc++.h>

using namespace std;
using cp::disjset::DSU;

int main() {
    int n;
    cin >> n;

    auto dsu = DSU(26);
    auto seen = vector<bool>(26);

    for (string s; cin >> s; ) {
        auto x = s[0] - 'a';
        for (const auto c : s) {
            seen[c - 'a'] = true;
            dsu.unite(x, c - 'a');
        }
    }

    set<int> s;
    for (int i = 0; i < 26; i++) {
        if (seen[i]) {
            s.insert(dsu.find(i));
        }
    }

    cout << s.size() << "\n";
}