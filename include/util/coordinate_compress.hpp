// TODO: change to template.

// no map from val to coord.
// only idx -> coord.


#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;
const int N = 100100;
int values[N]; // assume filled
int cmprsd[N]; // filled by compress()
pair<int,int> helper[N]; // initially blank     [val, idx] -> [coord, idx], then cmprsd[idx] = coord
int n;
void compress(void) {
    for (int i = 0; i < n; i++) { // (uncompressed coordinate, original index)
        helper[i].first = values[i];
        helper[i].second = i;
    }
    stable_sort(helper,helper+n); // sort by uncompressed coordinate
    for (int i = 0; i < n; i++) {
        // overwrite uncompressed coordinates with compressed
        helper[i].first = i;
        // unsort using original index for reverse lookup
        cmprsd[helper[i].second] = i;
        // warning: unequal compressed coordinates assigned to equal uncompressed coordinates
        // this may or may not be desirable
    }
}


// map ver.
#include <map>
#include <vector>
// coordinates-> (compressed coordinates).
map<int, int> compressed;
void compress(vector<int>& values) { // note reference
    for (int val : values) {
        compressed[val] = 0;
    }
    int cnt = 0;
    for (auto& entry : compressed) {
        entry.second = cnt++;
    }
    for (int &val : values) { // note reference
        val = compressed[val]; // overwrites vector
    }
}