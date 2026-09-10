#pragma once

#include <geom/point.hpp>

#include <cmath>
#include <vector>

namespace cp::geom {

/*
 * polygon area (trapezoidal rule)
 */

/// @brief Returns the area of a polygon.
/// @note `pts` must be in clockwise boundary order or counter-clockwise order.
template <typename T>
[[nodiscard]]
double area(const std::vector<Point<T>>& pts) {
    double res = 0;
    int n = static_cast<int>(pts.size());
    for (int i = 0; i < n; i++) {
        // signed area:
        //     (a        + b             ) * h/2 (/2 moved to the end)
        res += (pts[i].y + pts[(i+1)%n].y) * (pts[(i+1)%n].x - pts[i].x);
        // sometimes, h will be negative, which means we subtract area
    }
    return std::abs(res/2.0);
}


/*
 * polygon area (triangulation / cross product)
 */

/// @brief Returns the area of a polygon. (implementation uses triangulation and cross product)
/// @note `pts` must be in clockwise boundary order or counter-clockwise order.
template <typename T>
[[nodiscard]]
double area_tri(const std::vector<Point<T>>& pts) {
    double res = 0;
    int n = pts.size();

    // we omit i = 0 and i = n-1, which are degenerate triangles (area = 0)
    for (int i = 1; i < n-1; i++) {
        res += cross(pts[i] - pts[0], pts[i+1] - pts[0]);
    }
    return std::abs(res/2.0);
}


// Area of polygon below y = h.
double areaBelow(const vector<pt>& poly, double h) {
    vector<pt> clipped;

    const int n = poly.size();

    for (int i = 0; i < n; ++i) {
        const pt& a = poly[i];
        const pt& b = poly[(i + 1) % n];

        const bool ina = a.y <= h;
        const bool inb = b.y <= h;

        if (ina && inb) {
            // inside -> inside
            clipped.push_back(b);
        } else if (ina && !inb) {
            // inside -> outside
            // push_back intersection
            const double t = (h - a.y) / (b.y - a.y);

            clipped.push_back({
                a.x + t * (b.x - a.x),
                a.y + t * (b.y - a.y)
            });
        } else if (!ina && inb) {
            // outside -> inside
            // push_back intersection & b
            const double t = (h - a.y) / (b.y - a.y);

            clipped.push_back({
                a.x + t * (b.x - a.x),
                a.y + t * (b.y - a.y)
            });

            clipped.push_back(b);
        }
        // outside -> outside: add nothing
    }

    if (clipped.size() < 3) {
        return 0;
    }

    return std::abs(area(clipped));
}

} // namespace cp::geom
