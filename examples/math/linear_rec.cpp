/**
 * https://codeforces.com/problemset/problem/1117/D
 * AC: 265ms
 * 
 * Process:
 * 
 * initial:
 * I was thinking about summing the combinations. But iteration is
 * no faster than O(N) where N is up to 1e18, definitely TLE.
 * 
 * 
 * Seems that we should calculate it in O(log n) time.
 * -> linear recurrence with matrices?
 * 
 * dp(i): #config s.t. take up `i` units of space
 * 
 * note: what we are really interested in is the VALID different permutations
 *       of 1s and 0s, as in the sample. (VALID indicates which gems are split)
 * 
 * dp(0) = 1; dp(neg) = 0;
 * 
 * dp(i) = dp(i-1)      // the i th gem is not split
 *         + dp(i-M)    // the i th gem is split, which uses the last M spaces
 * 
 * Thus the matrix would look like this:
 * 
 *   (i-1)         (i-M)
 *      1 0 0 ... 0 1      // M numbers
 *      1 0 0 ... 0 0
 *      0 1 0 ... 0 0
 *      0 0 1 ... 0 0
 *      ...
 *      0 0 0 ... 1 0
 *      0 0 0 ... 0 1
 * 
 * Use LinearRecurrence and pass the first row as coeff.
 * 
 */
#include <bits/stdc++.h>
using namespace std;

#include <math/mod_int.hpp>
#include <math/linear_recurrence.hpp>

using namespace cp::math;
using mint = ModInt<1000*1000*1000+7>;

int main() {
    long long n, m;
    cin >> n >> m;

    vector<mint> coef(m);
    coef[0] = coef[m-1] = 1;

    LinearRecurrence<mint> rec(coef);
    cout << rec.nth(n);
}