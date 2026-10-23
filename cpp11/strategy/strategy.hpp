#pragma once
#include <memory>
#include <vector>
#include <algorithm>

class SortStrategy {
public:
    virtual ~SortStrategy() = default;
    virtual void sort(std::vector<int>&) const = 0;
};

class AscendingSort : public SortStrategy {
public:
    void sort(std::vector<int>& v) const override {
        std::sort(v.begin(), v.end());
    }
};

class DescendingSort : public SortStrategy {
public:
    void sort(std::vector<int>& v) const override {
        std::sort(v.begin(), v.end(), std::greater<int>());
    }
};

class Sorter {
public:
    void setStrategy(std::unique_ptr<SortStrategy> s) { strategy_ = std::move(s); }
    void sort(std::vector<int>& v) const { if (strategy_) strategy_->sort(v); }
private:
    std::unique_ptr<SortStrategy> strategy_;
};