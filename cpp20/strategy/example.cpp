#include "strategy.hpp"
#include <cassert>

int main() {
    std::vector<int> v{3, 1, 2};
    Sorter s;
    s.setStrategy(ascendingSort);
    s.sort(v);
    assert((v == std::vector<int>{1, 2, 3}));
    s.setStrategy(descendingSort);
    s.sort(v);
    assert((v == std::vector<int>{3, 2, 1}));
    return 0;
}