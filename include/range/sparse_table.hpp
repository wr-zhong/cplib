#pragma once

#include <vector>
#include <cassert>
#include <bit>

namespace cp::range {

/**
 * Merge has to be idempotent
 */
template<
    class T,
    class Merge
>
class SparseTable {
public:
    explicit SparseTable(
        const std::vector<T>& arr,
        Merge merge_op = {}
    );

    // query the merged result over [left, right)
    [[nodiscard]]
    T query(int left, int right) const;

private:
    std::vector<std::vector<T>> table_;
    Merge merge_;
};

// implementation

template<class T, class Merge>
SparseTable<T, Merge>::SparseTable(
    const std::vector<T>& arr,
    Merge merge_op
)
    : table_(cp::util::log2_floor(arr.size()) + 1),
      merge_(merge_op)
{
    assert(!arr.empty());

    // input the initial array
    table_[0] = arr;

    // for all intervals whose length are powers of 2
    for (size_t k = 1; k < table_.size(); ++k) {
        const int len = 1 << k;

        table_[k].resize(arr.size() - len + 1);

        for (size_t i = 0; i + len <= arr.size(); ++i) {
            // table_[i, i+len) is made up of table_[i,i+len/2) and table_[i+len/2,i+len)
            table_[k][i] =
                merge_(
                    table_[k - 1][i],
                    table_[k - 1][i + (len >> 1)]
                );
        }
    }
}

template<class T, class Merge>
T
SparseTable<T, Merge>::query(
    int left,
    int right
) const {
    assert(left < right);

    const int k = std::bit_width(static_cast<unsigned>(right - left)) - 1;

    return merge_(
        table_[k][left],
        table_[k][right - (1 << k)]
    );
}

}