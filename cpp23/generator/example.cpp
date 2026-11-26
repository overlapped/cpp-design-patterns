#include "generator.hpp"
#include <cassert>
#include <iostream>
#include <ranges>

int main() {
    // 1. Бесконечный Фибоначчи, берём первые 10
    std::vector<long long> fib;
    for (auto v : fibonacci() | std::views::take(10)) {
        fib.push_back(v);
    }
    assert((fib == std::vector<long long>{0,1,1,2,3,5,8,13,21,34}));

    // 2. Конечный iota
    std::vector<int> seq;
    for (int v : iota(5, 10)) seq.push_back(v);
    assert((seq == std::vector<int>{5,6,7,8,9}));

    // 3. Простые числа, первые 5
    std::vector<int> p;
    for (int v : primes() | std::views::take(5)) p.push_back(v);
    assert((p == std::vector<int>{2,3,5,7,11}));

    // 4. Рекурсивный обход дерева
    TreeNode n1{1}, n2{2}, n3{3}, n4{4}, n5{5};
    n2.left = &n1; n2.right = &n3;
    n4.left = &n2; n4.right = &n5;
    std::vector<int> inorderVals;
    for (const auto* node : inorder(&n4)) {
        inorderVals.push_back(node->value);
    }
    assert((inorderVals == std::vector<int>{1,2,3,4,5}));

    std::cout << "std::generator: lazy sequences via co_yield\n";
    return 0;
}