#pragma once

#include <vector>

namespace cp::math{

/// @brief Returns if `x` is a prime.
///
/// O(sqrt(n)), enough for single primality testing.
bool is_prime(int x) {
    if (x < 2) return false;

    for (int f = 2; f*f <= x; f++) {
        if (x % f == 0) return false;
    }
    
    return true;
}

/// @brief Returns prime factors in increasing order, including multiplicities.
///
/// eg, x = 12 -> { 2, 2, 3 }. For single factorization. O(sqrt(x)) time.
std::vector<int> prime_factorize(int x) {
    std::vector<int> factors;
    for (int f = 2; f*f <= x; f++) {
        while (x % f == 0) {
            factors.push_back(f);
            x /= f;
        }
    }

    if (x != 1) {
        factors.push_back(x);
    }

    return factors;
}

/**
 * gcd(a, b)
 * a >= 0 and b >= 0
 * C++17: gcd is defined in <numeric>
 */
constexpr long long gcd_(long long a, long long b) {
    return b ? gcd_(b, a % b) : a;
}

/**
 * lcm(a, b)
 * a >= 0 and b >= 0
 * C++17: lcm is defined in <numeric>
 */
constexpr long long lcm_(long long a, long long b) noexcept {
    if (a == 0 || b == 0) {
        return 0;
    }
    return a / gcd_(a, b) * b;
}


/**
 * Extended Euclidean Algorithm
 */

/// @brief Computes gcd(a, b) and coefficients x, y such that
/// `a*x + b*y = gcd(a, b)`.
/// @return gcd(a, b).
long long extended_gcd(long long a, long long b,
                       long long& x, long long& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    long long x1, y1;
    long long d = extended_gcd(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return d;
}

} // namespace cp::maths
