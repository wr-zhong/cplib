#pragma once

#include <geom/point.hpp>

#include <cmath>

namespace cp::geom {

/// @brief Rotates `p` counter-clockwise by `theta` radians about the origin.
/// @return The new point.
Point<double> rotate(const Point<double>& p, double theta) {
    Point<double> res;
    res.x = p.x * std::cos(theta) - p.y * std::sin(theta);
    res.y = p.x * std::sin(theta) + p.y * std::cos(theta);
    return res;
}

/// @brief Rotates `p` counter-clockwise by `theta` radians about point `o`.
/// @return The new point.
Point<double> rotate(const Point<double>& p, double theta, const Point<double>& o) {
    // math: subtract `o`, rotate, then add `o` back
    return rotate(p - o, theta) + o;
}

/// @brief Returns the CCW rotation angle `theta` that makes line `ab` horizontal.
///
/// ie, rotating `a` CCW by `theta` about `b` makes line `ab` horizontal.
[[nodiscard]]
double horizontal_angle(const Point<double>& a, const Point<double>& b) {
    return -std::atan2(a.y - b.y, a.x - b.x);
}

/// @brief Returns the CCW rotation angle `theta` that makes line `ab` vertical.
///
/// ie, rotating `a` CCW by `theta` about `b` makes line `ab` vertical.
double vertical_angle(const Point<double>& a, const Point<double>& b) {
    return std::atan2(a.x - b.x, a.y - b.y);
}

} // namespace cp::geom
