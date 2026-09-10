#pragma once

namespace cp::util {

/**
 * Returns the smallest x in [lo, hi] such that pred(x) is true.
 * CAUTION: Note the precondition! pred(lo) == false!
 *
 * Preconditions:
 * - pred is monotonic (FFFTTT)
 * - pred(lo) == false
 * - pred(hi) == true
 *
 * Complexity:
 * O(log(hi - lo))
 */
template <class Pred>
long long first_true(long long lo, long long hi, Pred&& pred) {
    while (lo < hi) {
        auto mid = lo + (hi - lo) / 2;
        if (pred(mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    return lo;
}

/**
 * Returns the largest x in [lo, hi] such that pred(x) is true.
 * CAUTION: Consider whether `lo` can be 0!
 *
 * Preconditions:
 * - pred is monotonic (TTTFFF)
 * - pred(lo) == true
 * - pred(hi) == false
 *
 * Complexity:
 * O(log(hi - lo))
 */
template<class Pred>
long long last_true(long long lo, long long hi, Pred&& pred) {
    while (lo < hi) {
        auto mid = lo + (hi - lo + 1) / 2;
        if (pred(mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }

    return lo;
}

template <typename Pred>
double first_true(double lo, double hi, Pred&& check) {
    for (int it = 0; it < 70; ++it) {
        const double mid = (lo + hi) / 2;
        if (check(mid)) {
            hi = mid;
        } else {
            lo = mid;
        }
    }
    return hi;
}

template <typename Pred>
double last_true(double lo, double hi, Pred&& check) {
    for (int it = 0; it < 70; ++it) {
        const double mid = (lo + hi) / 2;
        if (check(mid)) {
            lo = mid;
        } else {
            hi = mid;
        }
    }
    return lo;
}

} // namespace cp::util
