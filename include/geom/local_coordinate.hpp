#pragma once

#include "point.hpp"

#include <cmath>

namespace cp::geom {

// dot product
template <typename T>
T dot(const Point<T>& a, const Point<T>& b) {
    return a.x * b.x + a.y * b.y;
}

struct LocalCoordinate {
    using pt = Point<double>;

    pt origin;
    pt ex;  // local x-axis
    pt ey;  // local y-axis

    /// @brief Constructs a coordinate system with `a -> b` as the x-axis.
    LocalCoordinate(const pt& a, const pt& b)
    : origin(a) {
        const pt d = b - a;
        const double len = std::hypot(d.x, d.y);

        ex = {d.x / len, d.y / len};
        ey = {-ex.y, ex.x};
    }

    /// @brief Converts a global point to local coordinates.
    [[nodiscard]]
    pt toLocal(const pt& p) const {
        const pt d = p - origin;
        return {dot(d, ex), dot(d, ey)};
    }

    /// @brief Converts a local point to global coordinates.
    [[nodiscard]]
    pt toGlobal(const pt& p) const {
        return {
            origin.x + ex.x * p.x + ey.x * p.y,
            origin.y + ex.y * p.x + ey.y * p.y
        };
    }

    /// @brief Returns the local x-coordinate of a global point.
    [[nodiscard]]
    double x(const pt& p) const {
        return dot(p - origin, ex);
    }

    /// @brief Returns the local y-coordinate of a global point.
    [[nodiscard]]
    double y(const pt& p) const {
        return dot(p - origin, ey);
    }
};

} // namespace cp::geom
