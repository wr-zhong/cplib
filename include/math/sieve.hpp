#pragma once

#include <vector>

namespace cp::math {

/**
 * Sieve of Eratosthenes. For testing all primes up to `n`.
 */
class PrimeSieve {
public:
    explicit PrimeSieve(int n)
        : not_prime_(n + 1, false) {
        sieve();
    }

    [[nodiscard]]
    bool is_prime(int x) const {
        return x >= 2 && !not_prime_[x];
    }

    [[nodiscard]]
    const std::vector<int>& primes() const {
        return primes_;
    }

private:
    std::vector<bool> not_prime_;
    std::vector<int> primes_;

    void sieve() {
        int n = static_cast<int>(not_prime_.size()) - 1;

        if (n >= 0) {
            not_prime_[0] = true;
        }
        if (n >= 1) {
            not_prime_[1] = true;
        }

        for (int i = 2; i <= n; i++) {
            if (!not_prime_[i]) {
                primes_.push_back(i);

                if (1LL * i * i <= n) {
                    for (int j = i * i; j <= n; j += i) {
                        not_prime_[j] = true;
                    }
                }
            }
        }
    }
};

} // namespace cp::math
