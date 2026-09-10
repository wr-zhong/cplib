#include <iostream>
#include <version>

int main() {
    std::cout << __cplusplus << '\n';
    std::cout << __GNUC__ << '.' << __GNUC_MINOR__ << '\n';
}
