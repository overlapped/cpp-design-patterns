#pragma once
#include <generator>
#include <coroutine>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

// Генератор на корутинах C++23: std::generator<T> — ленивая
// последовательность, которая вычисляет следующий элемент только
// при запросе. В отличие от std::vector, не хранит все элементы
// в памяти — O(1) памяти вместо O(N) [citation:1].
//
// std::generator удовлетворяет input_range, поэтому работает
// с range-based for и std::views::take [citation:2][citation:5].

// 1. Бесконечная последовательность Фибоначчи
inline std::generator<long long> fibonacci() {
    long long a = 0, b = 1;
    while (true) {
        co_yield a;
        long long next = a + b;
        a = b;
        b = next;
    }
}

// 2. Конечная последовательность: числа от first до last-1
inline std::generator<int> iota(int first, int last) {
    for (int i = first; i < last; ++i) {
        co_yield i;
    }
}

// 3. Генератор простых чисел (бесконечный)
inline std::generator<int> primes() {
    co_yield 2;
    for (int n = 3; ; n += 2) {
        bool isPrime = true;
        for (int d = 3; d * d <= n; d += 2) {
            if (n % d == 0) { isPrime = false; break; }
        }
        if (isPrime) co_yield n;
    }
}

// 4. Генератор, который может бросить исключение
inline std::generator<int> throwAfter(int n) {
    for (int i = 0; i < n; ++i) co_yield i;
    throw std::runtime_error("generator exhausted with error");
}

// 5. Рекурсивный генератор для обхода дерева (in-order)
struct TreeNode {
    int value;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

inline std::generator<const TreeNode*> inorder(const TreeNode* node) {
    if (!node) co_return;
    co_yield std::ranges::elements_of(inorder(node->left));
    co_yield node;
    co_yield std::ranges::elements_of(inorder(node->right));
}