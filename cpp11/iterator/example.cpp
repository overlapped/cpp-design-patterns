#include "iterator.hpp"
#include <cassert>

int main() {
    int sum = 0;
    for (int v : IntRange(1, 5)) sum += v;
    assert(sum == 10); // 1+2+3+4
    return 0;
}