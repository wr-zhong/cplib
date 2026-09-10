/**
 * https://codeforces.com/problemset/problem/1366/D
 * AC: 1281ms
 * 
 * 
 * number that has only one prime fator -> no such pair
 * 
 * Note that we are considering the prime factors (ie, divisors) of a.
 * 
 * Key:
 * gcd(d1 + d2, a) = 1 <=> no divisors in a divides (d1 + d2).
 * 
 * If we group all pow of p_i for an i into the same divisor (d1 or d2),
 * then for any p_i which is a divisor pf a, it divides exactly one of
 * d1 and d2 (whichever has p_i).
 * Thus no divisor in `a` divides both d1 and d2 (ie, d1 + d2 is not divisible),
 * so we have our d1 and d2.
 * 
 * impl:
 * run sieve up to 1e7 for fast factorization of a_i.
 * Then take any valid group, say d1 is p_1^alpha1, d2 = rest.
 */

#include <bits/stdc++.h>
using namespace std;

#include <math/spf_sieve.hpp>
using namespace cp::math;

int main() {
    int n;
    cin >> n;

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    SPFSieve spf(1000*1000*10);

    vector<int> res1, res2;
    
    for (int i = 0; i < n; i++) {
        auto fac = spf.factorize(a[i]);
        int first = fac[0];
        long long d1 = first;
        // #factors is not n...
        for (auto it = ++fac.begin(); it != fac.end(); ++it) {
            if (*it != first) break;

            d1 *= *it;
        }

        if (d1 == a[i]) {
            // only one factor
            res1.push_back(-1);
            res2.push_back(-1);
        } else {
            res1.push_back(d1);
            res2.push_back(a[i] / d1);  // must be int
        }
    }

    for (auto i : res1) {
        cout << i << ' ';
    }
    cout << '\n';

    for (auto i : res2) {
        cout << i << ' ';
    }
    cout << '\n';

}
