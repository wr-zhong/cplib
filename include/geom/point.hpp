#pragma once

#include <cmath>
#include <concepts>
#include <compare>

namespace cp::geom {

// fwd declaration
template <typename T>
struct Point;

inline constexpr double EPS = 1e-8;

/// @return if `x` is equivalent to `0`.
template <typename T>
[[nodiscard]]
bool zero(T x) {
    if constexpr (std::floating_point<T>) {
        return std::abs(x) <= EPS;
    } else {
        return x == 0;
    }
}

/// @brief Returns the 2D cross product of two vectors.
///
/// The points are interpreted as vectors from the origin.
/// The result is the signed area of the parallelogram spanned by `a` and `b`.
///
/// Positive if `b` is counter-clockwise from `a`, negative if clockwise,
/// and zero if the vectors are collinear.
template <typename T>
[[nodiscard]]
constexpr T cross(const Point<T>& a, const Point<T>& b) {
    return a.x * b.y - a.y * b.x;
}

/// @return 1 if `x` is positve, -1 if `x` is negative, 0 if `x` is zero.
template <class T>
int sign(T x) {
    if constexpr (std::floating_point<T>) {
        if (x < -EPS) return -1;
        if (x > EPS) return 1;
        return 0;
    } else {
        return (x > 0) - (x < 0);
    }
}

/// @brief counter-clockwise
/// @return 1, -1 or 0: left, right or straight. (the sign of the cross product)
template <typename T>
int ccw(const Point<T>& a, const Point<T>& b, const Point<T>& c) {
    return sign((b - a).cross(c - a));  // ab cross ac
}

/// @return the distance between `a` and `b`.
template <typename T>
[[nodiscard]]
double dist(const Point<T>& a, const Point<T>& b) {
    const double dx = static_cast<double>(a.x) - static_cast<double>(b.x);
    const double dy = static_cast<double>(a.y) - static_cast<double>(b.y);
    return std::hypot(dx, dy);
}

/// @return the squared distance between `a` and `b`.
template <typename T>
[[nodiscard]]
T distsq(const Point<T>& a, const Point<T>& b) {
    const T dx = a.x - b.x;
    const T dy = a.y - b.y;
    return dx*dx + dy*dy;
}

template <class T = double>
struct Point {
    using value_type = T;
    using pt = Point<T>;    // a public in-class alias

    T x{};
    T y{};

    constexpr Point() = default;
    constexpr Point(T _x, T _y) : x(_x), y(_y) {}

    constexpr pt& operator+=(const pt& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr pt& operator-=(const pt& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    [[nodiscard]]
    friend constexpr pt operator+(pt lhs, const pt& rhs) {
        return lhs += rhs;
    }

    [[nodiscard]]
    friend constexpr pt operator-(pt lhs, const pt& rhs) {
        return lhs -= rhs;
    }

    // behaves the same as std::pair. No EPS.
    constexpr auto operator<=>(const pt&) const = default;

    /// @brief Returns the 2D cross product of two vectors.
    ///
    /// The arguments are interpreted as vectors, even though they are
    /// represented by `Point`. The result is the signed area of the
    /// parallelogram spanned by `a` and `b`.
    ///
    /// Positive if `rhs` is counter-clockwise from `*this`, negative if clockwise,
    /// and zero if the vectors are collinear.
    [[nodiscard]]
    constexpr T cross(const pt& rhs) const {
        return x * rhs.y - y * rhs.x;
    }

};

} // namespace cp::geom