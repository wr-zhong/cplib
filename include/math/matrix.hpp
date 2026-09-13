#pragma once

#include <vector>
#include <concepts>

namespace cp::math {

template <class T>
concept MatrixElement = requires(T a, T b) {
    T{0};
    T{1};

    { a * b } -> std::convertible_to<T>;
    { a + b } -> std::convertible_to<T>;
};

// Implementation for square matrices.
// class `T` has to support `*`, `+`, `+=`, `T{1}`. eg, `cp::maths::ModInt`
template <MatrixElement T = long long>
struct Matrix {
    int n_;
    std::vector<std::vector<T>> entries_;

    explicit Matrix(int n)
    : n_(n)
    , entries_(n_, std::vector<T>(n_, T{0})) {} // all entries initialized to 0.

    // matrix multiplication. O(n^3).
    [[nodiscard]]
    Matrix operator*(const Matrix &other) const {
        Matrix res(n_);
        for (int i = 0; i < n_; i++) {
            for (int j = 0; j < n_; j++) {
                for (int k = 0; k < n_; k++) {
                    res.entries_[i][j] = res.entries_[i][j] + entries_[i][k] * other.entries_[k][j];
                }
            }
        }
        return res;
    }

    // Returns the resulting matrix of `(*this)^k`.
    // `k >= 0`. O(log k).
    // note: Matrix is a square matrix.
    [[nodiscard]]
    Matrix operator^(long long k) const {
        Matrix res = this->identity();
        Matrix a = *this;
        // building up in powers of two
        while (k > 0) {
            if (k & 1) {
                res = res * a;
            }

            k /= 2;
            if (k > 0) {
                a = a * a;
            }
        }

        return res;
    }

private:
    // returns an identity matrix of the same dimension as `*this`.
    [[nodiscard]]
    Matrix identity() const {
        Matrix ret(n_);
        for (int i = 0; i < n_; i++) {
            ret.entries_[i][i] = T{1};
        }
        return ret;
    }
};

} // namespace cp::maths
