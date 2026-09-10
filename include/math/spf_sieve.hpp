#pragma once

#include <vector>

namespace cp::math {

/**
 * Sieve of Eratosthenes (SPF). For all prime factorization up to `n`.
 */
class SPFSieve {
public:
    // sieve up to `n`. O(n log log n), comfortable for `n <= 1e7`.
    // you are always more limited by memory.
    explicit SPFSieve(int n)
        : lp_(n + 1) {
        sieve();
    }

    // factorize any `1 < x <= n`.
    // returns a vector of prime factors in ascending order with duplicates (multiplicity).
    [[nodiscard]]
    std::vector<int> factorize(int x) const {
        std::vector<int> ret;
        while (x > 1) {
            ret.push_back(lp_[x]);
            x /= lp_[x];
        }
        return ret;
    }

private:
    // least prime of i
    std::vector<int> lp_;

    void sieve() {
        int n = static_cast<int>(lp_.size()) - 1;

        // init
        for (int i = 2; i <= n; i++) {
            lp_[i] = i;
        }

        for (int i = 2; i * i <= n; i++) {
            if (lp_[i] == i) {  // prime
                // start cross out from i*i
                for (int j = i * i; j <= n; j += i) {
                    if (lp_[j] == j) {
                        lp_[j] = i;
                    }
                }
            }
        }
    }
};

using Sieve = SPFSieve;

} // namespace cp::math
