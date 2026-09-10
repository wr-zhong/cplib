#pragma once

#include <vector>
#include <functional>
#include <cassert>


/**
 * all responsibilities are [l, r)
 * leaf: r - l == 1
 * arr refers to the original data array / vector
 */

namespace cp::range {

/**
 * point update
 * range query
 * 
 * - `Merge` must be associative.
 * - indices are 0-based.
 */
template<
    class T,
    class Merge = std::plus<T>
>
class RangeTree {
public:
    /**
     * Preconditions:
     * - return type of merge_op should be convertable to T (default std::plus<T>).
     */
    explicit RangeTree(const std::vector<T>& arr, Merge merge_op = {})
    : n_(arr.size()), tree_(4 * n_), merge_(merge_op) {
        assert(!arr.empty());
        build(arr, 1, 0, n_);  // tree_ rooted at 1
    }

    /**
     * pos is the index in the array (0-based)
     * value is the value that the p-th element will be updated to
     */
    void update(int pos, T value) {
        update(pos, value, 1, 0, n_);
    }

    /**
     * Returns the merged result of [left, right). (0-based)
     * 
     * Preconditions:
     * - 0 <= left < right <= n
     */
    [[nodiscard]]
    T query(int left, int right) const {
        assert(left < right);
        return query(left, right, 1, 0, n_);
    }

private:
    int n_;
    // 4 * n_ is a convenient upper bound
    // tree_ size is actually bounded by 2 * [the smallest power of 2 >= n_ - 1]. (2 * std::bitceil(n_) - 1)
    std::vector<T> tree_;

    Merge merge_;

    /**
     * Builds the subtree rooted at index node in tree_.
     * node is the index in the tree_, rooted at 1 so children are 2i and 2i+1
     * Instead of explicitly storing each node's range of responsibility [l,r), we calculate it on the way down.
     * The root node is responsible for [0, n) in arr.
     */
    void build(
        const std::vector<T>& arr,
        int node,
        int l,
        int r
    ) {
        if (r - l == 1) {
            // leaf
            tree_[node] = arr[l];
            return;
        }

        int mid = l + (r - l) / 2;

        build(arr, node * 2, l, mid);
        build(arr, node * 2 + 1, mid, r);

        tree_[node] = merge_(tree_[node * 2], tree_[node * 2 + 1]);
    }

    /**
     * pos is the index in the array (0-based)
     * value is the value that the p-th element will be updated to
     * node is the index in tree_
     * [l, r) is responsibility in the array. The root is reponsible for [0, n)
     */
    void update(
        int pos,
        T value,
        int node,
        int l,
        int r
    ) {
        if (r - l == 1) {
            tree_[node] = value;
            return;
        }

        int mid = l + (r - l) / 2;

        if (pos < mid) {
            // go left
            update(pos, value, node * 2, l, mid);
        } else {
            // go right
            update(pos, value, node * 2 + 1, mid, r);
        }

        tree_[node] = merge_(tree_[node * 2], tree_[node * 2 + 1]);
    }

    /**
     * query the sum over [ql, qr) (0-based)
     */
    [[nodiscard]]
    T query(
        int ql,
        int qr,
        int node,
        int l,
        int r
    ) const {
        if (l == ql && r == qr) {
            return tree_[node];
        }

        int mid = l + (r - l) / 2;

        if (qr <= mid) {
            return query(ql, qr, node * 2, l, mid);
        }

        if (ql >= mid) {
            return query(ql, qr, node * 2 + 1, mid, r);
        }

        return merge_(query(ql, mid, node * 2, l, mid),
            query(mid, qr, node * 2 + 1, mid, r));
    }
};

} // namespace cp::range
