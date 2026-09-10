#pragma once

namespace cp::util {

// std::bit_width(x) - 1 in C++20
inline int log2_floor(unsigned x) noexcept {
    return 31 - __builtin_clz(x);
}

// std::bit_width(x) - 1 in C++20
inline int log2_floor(unsigned long long x) {
    return 63 - __builtin_clzll(x);
}

}