#include "iterator.hpp"
#include <cassert>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> v;
    for (int x : IntRange(1, 4)) v.push_back(x);
    assert((v == std::vector<int>{1, 2, 3}));

    // Работает с std::ranges
    int sum = 0;
    for (int x : IntRange(1, 5) | std::views::transform([](int n) { return n * 2; }))
        sum += x;
    assert(sum == 2 + 4 + 6 + 8);
    return 0;
}