#include <graph/flow_network.hpp>

// simply copy what you need
namespace flow_network_utils {

//////////// declarations ////////////
// not for copying!

cp::graph::FlowNetwork h;
int n, m;
int id(int i, int j);


//////////////////////////////////////

/**
 * grid 4-adj: add_edge
 */

int dr[] = {1, -1, 0, 0};
int dc[] = {0, 0, 1, -1};
auto grid_connect_adj = [&](int i, int j) {
    for (int k = 0; k < 4; k++) {
        int a = i+dr[k], b = j+dc[k];
        if (a < 0 || a >= n || b < 0 || b >= n)
            continue;
        
        if ( /* connect */true )   // change this
            h.add_edge(id(i, j), id(a, b), 1);
    }
};


} // namespace flow_network_utils
