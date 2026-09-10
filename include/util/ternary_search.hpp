#pragma once

namespace cp::util {

/**
 * TS on long long
 */

/**
 * Returns an `x` in [lo, hi] such that `f(x)` is minimised.
 * `f` should be bitonic on [lo, hi] with a minimum plateau.
 */
template <typename F>
long long ternary_search_min(long long lo, long long hi, F&& f) {
    while (hi - lo > 2) {
        const long long mid1 = lo + (hi - lo) / 3;
        const long long mid2 = hi - (hi - lo) / 3;

        if (f(mid1) < f(mid2)) {
            hi = mid2 - 1;
        } else {
            lo = mid1 + 1;
        }
    }

    long long res = lo;
    for (long long x = lo + 1; x <= hi; ++x) {
        if (f(x) < f(res)) res = x;
    }
    return res;
}

/**
 * Returns an `x` in [lo, hi] such that `f(x)` is maximised.
 * `f` should be bitonic on [lo, hi] with a maximum plateau.
 */
template <typename F>
long long ternary_search_max(long long lo, long long hi, F&& f) {
    while (hi - lo > 2) {
        const long long mid1 = lo + (hi - lo) / 3;
        const long long mid2 = hi - (hi - lo) / 3;

        if (f(mid1) > f(mid2)) {
            hi = mid2 - 1;
        } else {
            lo = mid1 + 1;
        }
    }

    long long res = lo;
    for (long long x = lo + 1; x <= hi; ++x) {
        if (f(x) > f(res)) res = x;
    }
    return res;
}

/**
 * TS on double
 */

template <typename F>
double ternary_search_min(double lo, double hi, F&& f) {
    for (int it = 0; it < 70; ++it) {
        const double mid1 = (2 * lo + hi) / 3;
        const double mid2 = (lo + 2 * hi) / 3;

        if (f(mid1) < f(mid2)) {
            hi = mid2;
        } else {
            lo = mid1;
        }
    }
    return (lo + hi) / 2;
}

template <typename F>
double ternary_search_max(double lo, double hi, F&& f) {
    for (int it = 0; it < 70; ++it) {
        const double mid1 = (2 * lo + hi) / 3;
        const double mid2 = (lo + 2 * hi) / 3;

        if (f(mid1) > f(mid2)) {
            hi = mid2;
        } else {
            lo = mid1;
        }
    }
    return (lo + hi) / 2;
}

} // namespace cp::util