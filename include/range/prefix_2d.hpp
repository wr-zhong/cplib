#pragma once

#include <vector>

namespace cp::range {

template<class T>
class PrefixSum2D {
public:
    /**
     * Preconditions:
     * - matrix is non-empty
     * - matrix is rectangular
     */
    explicit PrefixSum2D(const std::vector<std::vector<T>>& matrix) {
        build(matrix);
    }

    /**
     * Returns the sum of the matrix specified by indices of top left and bottom right.
     * 0-indexed.
     */
    [[nodiscard]]
    T sum(int top, int left, int bottom, int right) const;

private:
    std::vector<std::vector<T>> prefix_;

    void build(const std::vector<std::vector<T>>& matrix);
};

// implementations

/**
 * Builds the prefix_ matrix
 */
template<class T>
inline
void
PrefixSum2D<T>::build(
    const std::vector<std::vector<T>>& matrix
) {
    const int rows = static_cast<int>(matrix.size());
    const int cols = static_cast<int>(matrix[0].size());

    prefix_.assign(rows + 1, std::vector<T>(cols + 1, T{}));

    for (int r = 1; r <= rows; ++r) {
        for (int c = 1; c <= cols; ++c) {
            prefix_[r][c] =
                prefix_[r - 1][c]
                + prefix_[r][c - 1]
                - prefix_[r - 1][c - 1]
                + matrix[r - 1][c - 1];
        }
    }
}

template<class T>
inline
T
PrefixSum2D<T>::sum(
    int top,
    int left,
    int bottom,
    int right
) const {
    return prefix_[bottom + 1][right + 1]
        - prefix_[top][right + 1]
        - prefix_[bottom + 1][left]
        + prefix_[top][left];
}

} // namespace cp::range
