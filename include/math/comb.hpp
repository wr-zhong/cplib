#pragma once

#include <math/mod_int.hpp>

/// @brief Query comb(n, k) modulo `MOD`.
///
/// O(N + log`MOD`) precompt, O(1) per query.
/// @tparam MOD a prime
template<int MOD>
class Comb {
    using mint = cp::math::ModInt<MOD>;
public:
    /// @brief Support all comb(n, k) query for n up to `N`.
    explicit Comb(int N)
    : fact_(N + 1)
    , inv_fact_(N + 1) {
        fact_[0] = 1;
        for (int i = 1; i <= N; i++) {
            fact_[i] = fact_[i - 1] * i;
        }

        inv_fact_[N] = fact_[N].inv();
        for (int i = N; i > 0; i--) {
            // i * inv[i!] = i * (inv[i] * inv[(i-1)!]) = inv[(i-1)!]
            inv_fact_[i - 1] = inv_fact_[i] * i;
        }
    }

    [[nodiscard]]
    mint operator()(int n, int k) const {
        if (k < 0 || k > n) {
            return 0;
        }
        return fact_[n] * inv_fact_[k] * inv_fact_[n-k];
    }

private:
    std::vector<mint> fact_;        // fact_[k]: k factorial
    std::vector<mint> inv_fact_;    // inv_fact_[k]: the modular inverse of k factorial
};