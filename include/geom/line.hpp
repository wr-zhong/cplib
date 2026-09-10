#pragma once

#include <geom/point.hpp>

#include <cassert>
#include <cmath>

namespace cp::geom {

inline constexpr double EPS = 1e-8;

struct Line {
    // ax + by + c = 0.
    double a, b, c;

    constexpr Line(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {}

    template <typename T>
    constexpr Line(const Point<T>& p, const Point<T>& q)
    : a(static_cast<double>(p.y - q.y))
    , b(static_cast<double>(q.x - p.x))
    , c(static_cast<double>(p.x * q.y - q.x * p.y)) {}

    // Substitute `p` into the implicit form of the line.
    // The sign tells which side `p` is on.
    template <typename T>
    [[nodiscard]]
    constexpr double eval(const Point<T>& p) const {
        return a * p.x + b * p.y + c;
    }

    // Returns whether `p` is on the line.
    template <typename T>
    [[nodiscard]]
    constexpr bool contains(const Point<T>& p) const {
        return std::abs(eval(p)) <= EPS;
    }

    // Returns whether the two lines are parallel.
    [[nodiscard]]
    constexpr bool parallel(const Line& other) const {
        return std::abs(a * other.b - b * other.a) < EPS;
    }

    // Returns the intersection of the two lines.
    // Assumes the two lines are not parallel.
    [[nodiscard]]
    Point<double> intersect(const Line& other) const {
        assert(!parallel(other));

        const double d = a * other.b - b * other.a;
        const double x = (b * other.c - c * other.b) / d;
        const double y = (c * other.a - a * other.c) / d;

        return Point<double>(x, y);
    }
};

// Returns whether the two lines are parallel.
[[nodiscard]]
constexpr bool parallel(const Line& f, const Line& g) {
    return std::abs(f.a * g.b - f.b * g.a) < EPS;
}

// Returns the intersection of the two lines.
// Assumes the two lines are not parallel.
[[nodiscard]]
Point<double> intersect(const Line& f, const Line& g) {
    assert(!f.parallel(g));

    const double d = f.a * g.b - f.b * g.a;
    const double x = (f.b * g.c - f.c * g.b) / d;
    const double y = (f.c * g.a - f.a * g.c) / d;

    return Point<double>(x, y);
}

} // namespace cp::geom
