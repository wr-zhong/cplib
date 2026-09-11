#pragma once

#include "scc.hpp"

#include <cassert>

namespace cp::graph {

// Variable number must be 1-based.
struct TwoSAT {
    int vars_;   // number of variables

    SCC scc_;

    // `n` is the number of variables (not vertices, which is `2*n`).
    TwoSAT(int n) : vars_(n), scc_(2 * n) {}

    // returns the # of neg node of variable `x`.
    // variable number `x` is 1-based.
    int neg(int x) {
        // vertices 1..n are the variables, and (n+1)..2n their negatives.
        return x <= vars_ ? x + vars_ : x - vars_;
    }

    // add clause (x_a OR x_b)
    void add_or(int a, int b) {
        scc_.add_edge(neg(a), b);
        scc_.add_edge(neg(b), a);
    }

    // returns whether the 2-SAT is satisfiable
    bool is_satisfiable() {
        scc_.compute();

        for (int i = 1; i <= vars_; ++i) {
            if (scc_.scc[i] == scc_.scc[neg(i)]) return false;
        }

        return true;
    }

    /**
     * Returns a vector representing the solution.
     * Number of variables in `solution` is 0-based.
     */
    std::vector<bool> find_solution (void) {
        assert(is_satisfiable());
        std::vector<bool> solution(vars_);
        // Kosaraju finds the SCCs in topological order
        // if F before T, then T // if T before F, then F
        for (int i = 1; i <= vars_; ++i) {
            solution[i - 1] = (scc_.scc[i] > scc_.scc[neg(i)]);
        }
        return solution;
    }
};

} // namespace cp::graph
