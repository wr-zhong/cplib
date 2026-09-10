#pragma once

#include <geom/point.hpp>

#include <vector>
#include <algorithm>

namespace cp::geom::detail {

// change the while loop if you want to modify how collinear vertices are removed
template <typename T>
vector<Point<T>> half_hull(const vector<Point<T>>& pts) {
    vector<Point<T>> res;

    for (int i = 0; i < static_cast<int>(pts.size()); i++) {
        // ccw >= 0 means most recent 2 pts in res and cur form a left turn / collinear; we don't want that
        while (res.size() >= 2 && ccw(res[res.size()-2], res[res.size()-1], pts[i]) >= 0) {
            res.pop_back();     // pop the previous one
        }
        res.push_back(pts[i]);
    }
    return res;
}

} // namespace cp::geom::detail

namespace cp::geom {

/*
 * convex hull
 */

/// @return the vertices of the convex hull in clockwise order. (Collinear vertices are removed)
template<typename T>
vector<Point<T>> convex_hull(vector<Point<T>> pts) { // copy pts to preserve original order
    if (pts.size() <= 1) {
        return pts;
    }

    std::sort(pts.begin(), pts.end());      // from leftmost to rightmost
    vector<Point<T>> top = detail::half_hull(pts);  // get top-half (no left turn)

    std::reverse(pts.begin(), pts.end());       // from rightmost to leftmost
    vector<Point<T>> bottom = detail::half_hull(pts);   // get bottom-half (still no left turn)
    
    // remove repeated endpoints
    top.pop_back();
    bottom.pop_back();

    // insert bottom after top
    top.insert(top.end(), bottom.begin(), bottom.end());
    return top;
}

} // namespace cp::geom
