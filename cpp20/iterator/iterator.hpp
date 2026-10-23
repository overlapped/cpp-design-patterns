#pragma once
#include <cstddef>
#include <iterator>

class IntRange {
public:
    IntRange(int begin, int end) : begin_(begin), end_(end) {}

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = int;
        using difference_type = std::ptrdiff_t;
        using pointer = const int*;
        using reference = int;

        Iterator(int v = 0) : value_(v) {}
        int operator*() const { return value_; }
        Iterator& operator++() { ++value_; return *this; }
        Iterator operator++(int) { auto tmp = *this; ++value_; return tmp; }
        bool operator==(const Iterator& o) const { return value_ == o.value_; }
    private:
        int value_;
    };

    Iterator begin() const { return Iterator(begin_); }
    Iterator end() const { return Iterator(end_); }

private:
    int begin_, end_;
};