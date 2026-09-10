#pragma once

#include <algorithm>
#include <vector>

namespace cp::math {

/// @brief Computes a single (`n` choose `k`) in O(k) time.
long long comb(int n, int k) {
    k = std::min(k, n - k);
    long long ans = 1;
    for (int i = 1; i <= k; i++) {
        // since comb(n,k) = comb(n, k-1) * (n-k+1) / k,
        // comb(n, k-1) * (n-k+1) / k must an fully integer operation.
        // This applies to all k.
        ans = ans * (n - i + 1) / i;
    }
    return ans;
}

/// @brief Computes `comb(i, j)` for all `i` in [0, n] and `j` in [0, k] in O(nk) time.
/// @return `comb[i][j]`.
std::vector<std::vector<long long>> comb_table(int n, int k) {
    std::vector<std::vector<long long>> c(n+1, std::vector<long long>(k+1));

    for (int i = 0; i <= n; i++) {
        c[i][0] = 1;
        for (int j = 1; j <= std::min(i,k); j++) {
            c[i][j] = c[i-1][j-1] + c[i-1][j];
        }
    }

    return c;
}

/// @brief Computes all comb(n, i) for `i` in [0, k]. Linear time.
/// @return `ret[i]`: comb(n, i).
std::vector<long long> comb_row(int n, int k) {
    k = std::min(k, n - k);
    std::vector<long long> c(k+1);
    c[0] = 1;

    for (int i = 0; i < k; i++) {
        c[i+1] = c[i] * (n-i) / (i+1);
    }

    return c;
}

} // namespace cp::math