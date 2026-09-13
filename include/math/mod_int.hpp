#pragma once

#include <cassert>
#include <iostream>

namespace cp::math {

/// @brief keep an `int` modulo `MOD`.
/// @tparam MOD the modulus of type `int`. Must be prime.
template <int MOD>
struct ModInt {
    int val;

    constexpr ModInt() : val(0) {}

    constexpr ModInt(long long x) {
        x %= MOD;
        if (x < 0) {
            x += MOD;
        }
        val = static_cast<int>(x);
    }

    [[nodiscard]]
    constexpr int value() const {
        return val;
    }

    constexpr ModInt& operator+=(const ModInt& other) {
        val += other.val;
        if (val >= MOD) {
            val -= MOD;
        }
        return *this;
    }

    constexpr ModInt& operator-=(const ModInt& other) {
        val -= other.val;
        if (val < 0) {
            val += MOD;
        }
        return *this;
    }

    constexpr ModInt& operator*=(const ModInt& other) {
        // avoid overflows
        val = static_cast<long long>(val) * other.val % MOD;
        return *this;
    }

    [[nodiscard]]
    friend constexpr ModInt operator+(ModInt lhs, const ModInt& rhs) {
        // note lhs is a copy
        lhs += rhs;
        return lhs;
    }

    [[nodiscard]]
    friend constexpr ModInt operator-(ModInt lhs, const ModInt& rhs) {
        lhs -= rhs;
        return lhs;
    }

    [[nodiscard]]
    friend constexpr ModInt operator*(ModInt lhs, const ModInt& rhs) {
        lhs *= rhs;
        return lhs;
    }

    friend constexpr bool operator==(const ModInt& lhs, const ModInt& rhs) {
        return lhs.val == rhs.val;
    }

    // don't need this in C++20
    friend constexpr bool operator!=(const ModInt& lhs, const ModInt& rhs) {
        return !(lhs == rhs);
    }

    friend std::ostream& operator<<(std::ostream& os, const ModInt& x) {
        return os << x.val;
    }

    // Computes `*this ^n`. O(log n) time.
    [[nodiscard]]
    constexpr ModInt pow(long long n) const {
        ModInt res = 1;
        ModInt base = *this;

        while (n > 0) {
            if (n & 1) {
                res *= base;
            }
            base *= base;
            n /= 2;
        }

        return res;
    }

    /// @brief Computes the modular inverse of `*this`. O(log `MOD`).
    /// @note `*this` != 0 (inverse is not defined).
    [[nodiscard]]
    constexpr ModInt inv() const {
        assert(val != 0);
        return pow(MOD-2);  // Fermat's Little Theorem
    }

    constexpr ModInt& operator/=(const ModInt& rhs) {
        return *this *= rhs.inv();
    }

    /// @note if b | a, then [(a / b) mod P] == [ModInt<P>(a) / ModInt<P>(b)].
    friend constexpr ModInt operator/(ModInt lhs, const ModInt& rhs) {
        lhs /= rhs;
        return lhs;
    }
};

} // namespace cp::math
