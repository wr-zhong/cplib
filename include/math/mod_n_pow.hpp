#pragma once

namespace cp::math {

/// @brief Returns the mathematical result of `a mod m`.
/// Useful when `a` may be negative.
///
/// @param a The dividend.
/// @param m The positive modulus. ie, `m > 0`
template <class T>
[[nodiscard]]
constexpr T actual_mod(T a, T m) noexcept {
    return ((a % m) + m) % m;
}

/**
 * fast exponentiate
 */

/// @brief Returns the result of `a^n mod m`. If a < 0, this returns the mathematical result.
///
/// Assumes `n >= 0` and `m > 0`. Runs in O(log n) time.
long long mod_pow(long long a, long long n, int m) noexcept {
    a = actual_mod(a, static_cast<long long>(m));

    long long ret = 1;
    while (n > 0) {
        if (n & 1) {
            ret = ret * a % m;
        }
        a = a * a % m;
        n /= 2;
    }
    return ret;
}

} // namespace cp::maths
