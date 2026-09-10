#pragma once

#include <math/matrix.hpp>

#include <vector>

namespace cp::math {

// class `T` has to support `*`, `+`, `+=`, `T{1}`. eg, `cp::maths::ModInt`
// template <MatrixElement T>   // c++20
template <class T>
class LinearRecurrence {
public:
    /// The initial (impulse) sequence of size k is fixed to [1,0,0,...,0]
    /// @param coeff coefficients of `a_{n-1}` to `a_{n-k}` in the recurrence. Might contains lots of 0s.
    explicit LinearRecurrence(std::vector<T> coeff)
    : transition_(build_transition(coeff)) {}

    [[nodiscard]]
    T nth(long long n) const {
        return (transition_ ^ n).entries_[0][0];
    }

    [[nodiscard]]
    T operator()(long long n) const {
        return nth(n);
    }

private:
    Matrix<T> transition_;
    // std::vector<T> initial_;

    // returns a transition matrix for a linear recurrence whose coefficients
    // of `a_{n-1}` to `a_{n-k}` are given by `coeff`.
    static Matrix<T> build_transition(const std::vector<T>& coeff) {
        int k = static_cast<int>(coeff.size());

        Matrix<T> ret(k);

        for (int i = 0; i < k; i++) {
            ret.entries_[0][i] = coeff[i];
        }

        for (int i = 1; i < k; i++) {
            ret.entries_[i][i - 1] = T{1};
        }

        return ret;
    }
};

} // namespace cp::maths
