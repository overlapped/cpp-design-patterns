#pragma once
#include <memory>
#include <vector>

class Node {
public:
    virtual ~Node() = default;
    virtual int size() const = 0;
};

class File : public Node {
public:
    explicit File(int s) : size_(s) {}
    int size() const override { return size_; }
private:
    int size_;
};

class Directory : public Node {
public:
    void add(std::unique_ptr<Node> n) { children_.push_back(std::move(n)); }
    int size() const override {
        int total = 0;
        for (const auto& c : children_) total += c->size();
        return total;
    }
private:
    std::vector<std::unique_ptr<Node>> children_;
};