#pragma once

#include <vector>

namespace cp::math {

/// @return the number of integers in `[1, N]` that is coprime to `d`.
/// @note O(sqrt(d) + r*2^r), where r is the number of distinct prime factors of `d`.
/// Usually dominated by sqrt(d). If you do need to speed up, consider precomputing
/// all primes <= sqrt(d), then test only on primes. But normally, you don't.
[[nodiscard]]
long long count_coprime(long long N, int d) {
    // Note: k and d are coprime iff no prime factor of d divides k

    std::vector<int> factors; // pf of d, without dup
    for (int f = 2; f*f <= d; f++) {
        if (d % f == 0)
            factors.push_back(f);
        while (d % f == 0) {
            d /= f;
        }
    }
    if (d != 1) {
        factors.push_back(d);
    }

    long long ans = 0;  // note N is added when S == 0

    int r = factors.size();

    // Use inc-exc to calculate #k is not divisible by any factor of d
    for (int S = 0; S < (1<<r); ++S) {
        // # of k that is divisible by the prod of chosen (distinct) factors
        // Note this is given by N / prod.
        long long prod = 1;

        for (int j = 0; j < r; ++j) if (S & (1<<j)) {
            prod *= factors[j];
        }

        auto cnt = N / prod;

        // C++20: std::popcount(S), defined in <bit>
        if (__builtin_popcount(S) & 1) {
            ans -= cnt;
        } else {
            ans += cnt;
        }
    }

    return ans;    
}

} // namespace cp::math
