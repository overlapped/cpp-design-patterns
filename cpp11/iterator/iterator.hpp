#pragma once
#include <cstddef>
#include <stdexcept>

class IntRange {
public:
    IntRange(int begin, int end) : begin_(begin), end_(end) {}

    class Iterator {
    public:
        Iterator(int v) : value_(v) {}
        int operator*() const { return value_; }
        Iterator& operator++() { ++value_; return *this; }
        bool operator!=(const Iterator& o) const { return value_ != o.value_; }
    private:
        int value_;
    };

    Iterator begin() const { return Iterator(begin_); }
    Iterator end() const { return Iterator(end_); }

private:
    int begin_, end_;
};