#pragma once
#include <functional>
#include <vector>
#include <algorithm>

class Sorter {
public:
    using SortFn = std::function<void(std::vector<int>&)>;

    void setStrategy(SortFn fn) { strategy_ = std::move(fn); }
    void sort(std::vector<int>& v) const { if (strategy_) strategy_(v); }

private:
    SortFn strategy_;
};

inline void ascendingSort(std::vector<int>& v) {
    std::sort(v.begin(), v.end());
}

inline void descendingSort(std::vector<int>& v) {
    std::sort(v.begin(), v.end(), std::greater<int>());
}