#pragma once

#include <vector>
#include <numeric>

namespace cp::disjset {

// Disjoint Set Union
// nodes are 0-indexed
class DSU {
public:
    std::vector<int> parent_;
    std::vector<int> size_;

    // 0-indexed
    explicit DSU(int n): parent_(n), size_(n, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int x);

    bool unite(int a, int b);
};

inline int DSU::find(int x) {
    if (parent_[x] == x) return x;

    return parent_[x] = find(parent_[x]);   // path compression
}

inline bool DSU::unite(int a, int b) {
    a = find(a);
    b = find(b);

    if (a == b) return false;

    if (size_[a] < size_[b]) {
        std::swap(a, b);
    }

    parent_[b] = a;
    size_[a] += size_[b];

    return true;
}

} // namespace cp::disjset
