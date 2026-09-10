#pragma once

#include <geom/point.hpp>
#include <geom/line.hpp>

namespace cp::geom {

inline constexpr double EPS = 1e-8;

struct HalfPlane {
    Line line;
    bool neg; // true: <= 0, false: >= 0

    constexpr HalfPlane(double a, double b, double c, bool neg)
    : line(a, b, c)
    , neg(neg) {}

    constexpr HalfPlane(const Point<double>& p, const Point<double>& q, bool neg)
    : line(p, q)
    , neg(neg) {}

    // returns if `p` is in half-plane.
    [[nodiscard]]
    bool contains(const Point<double>& p) const {
        auto res = line.eval(p);
        return neg ? res <= EPS : res >= -EPS;
    }
};

} // namespace cp::geom
