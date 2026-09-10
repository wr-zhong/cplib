namespace cp::graph {

/**
 * int u;
 * int v;
 * long long w;   // weight
 * 
 * an operator< on weight is defined for Kruskal.
 */
struct Edge {
    int u;  // from
    int v;  // to
    long long w;  // weight

    bool operator<(const Edge& rhs) const {
        return w < rhs.w;
    }
};



/**
 * int to;
 * long long w;
 */
struct AdjEdge {
    int to;
    long long w;
};

}