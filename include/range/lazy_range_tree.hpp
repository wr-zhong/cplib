#pragma once

#include <algorithm>
#include <vector>
#include <cassert>

/**
 * all responsibilities are [l, r)
 * leaf: r - l == 1
 * arr refers to the original data array / vector
 */

namespace cp::range {

/**
 * Only support sum query.
 * T should either be `long long` or `double`.
 */
template<class T = long long>
class LazyRangeTree {
public:
    explicit LazyRangeTree(const std::vector<T>& arr);

    void add(int left, int right, T value);

    [[nodiscard]]
    T sum(int left, int right);

    [[nodiscard]]
    T at(int pos);

private:
    int n_;

    std::vector<T> tree_;
    std::vector<T> lazy_;

    /**
     * Builds the subtree rooted at index node in tree_.
     * node is the index in the tree_, rooted at 1 so children are 2i and 2i+1
     * Instead of explicitly storing each node's range of responsibility [l,r), we calculate it on the way down.
     * The root node is responsible for [0, n) in arr.
     */
    void build(
        const std::vector<T>& arr,
        int node,
        int cl,
        int cr
    );

    void push_down(
        int node,
        int cl,
        int cr
    );

    void range_add(
        int left,
        int right,
        T value,
        int node,
        int cl,
        int cr
    );

    [[nodiscard]]
    T range_sum(
        int left,
        int right,
        int node,
        int cl,
        int cr
    );
    
    [[nodiscard]]
    T point_query(
        int pos,
        int node,
        int cl,
        int cr
    );

};

template<class T>
LazyRangeTree<T>::LazyRangeTree(
    const std::vector<T>& arr
)
    : n_(arr.size()),
      tree_(4 * n_),
      lazy_(4 * n_)
{
    assert(n_ > 0);
    build(arr, 1, 0, n_);
}

template<class T>
void LazyRangeTree<T>::add(
    int left,
    int right,
    T value
) {
    range_add(left, right, value, 1, 0, n_);
}

template<class T>
T LazyRangeTree<T>::sum(
    int left,
    int right
) {
    return range_sum(left, right, 1, 0, n_);
}

template<class T>
T LazyRangeTree<T>::at(
    int pos
) {
    return point_query(pos, 1, 0, n_);
}

template<class T>
void LazyRangeTree<T>::build(
    const std::vector<T>& arr,
    int node,
    int cl,
    int cr
) {
    if (cr - cl == 1) {
        // leaf
        tree_[node] = arr[cl];
        return;
    }

    int mid = cl + (cr - cl) / 2;

    build(arr, node * 2, cl, mid);
    build(arr, node * 2 + 1, mid, cr);

    tree_[node] = tree_[node * 2] + tree_[node * 2 + 1];
}

template<class T>
void LazyRangeTree<T>::push_down(
    int node,
    int cl,
    int cr
) {
    if (lazy_[node] == 0) return;

    if (cr - cl == 1) return;   // leaf

    int mid = cl + (cr - cl) / 2;
    T lazy = lazy_[node];

    tree_[node * 2] += lazy * (mid - cl);

    tree_[node * 2 + 1] += lazy * (cr - mid);

    lazy_[node * 2] += lazy;

    lazy_[node * 2 + 1] += lazy;

    lazy_[node] = 0;
}

template<class T>
void LazyRangeTree<T>::range_add(
    int left,
    int right,
    T value,
    int node,
    int cl,
    int cr
) {
    if (cl == left && cr == right) {
        // lazy
        tree_[node] += value * (right - left);
        lazy_[node] += value;
        return;
    }

    push_down(node, cl, cr);

    // update children
    int mid = cl + (cr - cl) / 2;

    if (left < mid) {
        range_add(left, std::min(right, mid), value, node * 2, cl, mid);
    }

    if (right > mid) {
        range_add(std::max(left, mid), right, value, node * 2 + 1, mid, cr);
    }

    // pull up: calculate parent after children have been updated.
    tree_[node] = tree_[node * 2] + tree_[node * 2 + 1];
}

template<class T>
T LazyRangeTree<T>::range_sum(
    int ql,
    int qr,
    int node,
    int cl,
    int cr
) {
    if (cl == ql && cr == qr) {
        return tree_[node];
    }

    push_down(node, cl, cr);

    int mid = cl + (cr - cl) / 2;

    if (qr <= mid) {
        return range_sum(ql, qr, node * 2, cl, mid);
    }

    if (ql >= mid) {
        return range_sum(ql, qr, node * 2 + 1, mid, cr);
    }

    return range_sum(ql, mid, node * 2, cl, mid)
        + range_sum(mid, qr, node * 2 + 1, mid, cr);
}

template<class T>
T LazyRangeTree<T>::point_query(
    int pos,
    int node,
    int cl,
    int cr
) {
    if (cr - cl == 1) {
        // leaf == pos
        return tree_[node];
    }

    push_down(node, cl, cr);

    // figure out which child is responsible for the index (pos) being updated
    int mid = cl + (cr - cl) / 2;
    if (pos < mid) {
        return point_query(pos, node * 2, cl, mid);
    } else {
        return point_query(pos, node * 2 + 1, mid, cr);
    }
}

} // namespace cp::range
