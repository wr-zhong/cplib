#pragma once

#include <geom/point.hpp>

namespace cp::geom {

inline constexpr double EPS = 1e-8;

// fwd declaration
template <typename T>
struct Segment;

/// @brief Returns whether all four endpoints of two segments are collinear.
///
/// Assumes both segments are non-degenerate (ie, no segment is a point).
template <typename T>
bool collinear(const Segment<T>& ab, const Segment<T>& cd) { // all four points collinear
    const auto& a = ab.first;
    const auto& b = ab.second;

    const auto& c = cd.first;
    const auto& d = cd.second;
    // check (ab, ac) and (ab, ad)
    return zero(cross(b - a, c - a)) &&
           zero(cross(b - a, d - a));
}

/// @return whether `ab` and `cd` intersect. Default to excluding non-proper intersections.
template <typename T>
[[nodiscard]]
bool intersect(const Segment<T>& ab, const Segment<T>& cd) {
    const auto& a = ab.first;
    const auto& b = ab.second;

    const auto& c = cd.first;
    const auto& d = cd.second;

    if (collinear(ab, cd)) {
        // check max dist against total dist
        const auto max_dist = std::max({dist(a, b), dist(a, c), dist(a, d),
                                    dist(b, c), dist(b, d), dist(c, d)});
        return max_dist < dist(a, b) + dist(c, d) + EPS;
    }

    // only proper intersections
    return ccw(a, b, c) * ccw(a, b, d) < 0 &&
           ccw(c, d, a) * ccw(c, d, b) < 0;

    // includes non-proper intersections (ie, looks like 'T' or 'L')
    // return ccw(a, b, c) * ccw(a, b, d) <= 0 &&
    //        ccw(c, d, a) * ccw(c, d, b) <= 0;
}

template <class T = double>
struct Segment {
    using value_type = T;
    using seg = Segment;

    Point<T> first;
    Point<T> second;

    constexpr Segment(Point<T> first, Point<T> second)
    : first(std::move(first))
    , second(std::move(second)) {}
};

} // namespace cp::geom
