#include "composite.hpp"
#include <cassert>

int main() {
    Directory root;
    root.add(std::make_unique<File>(3));
    auto sub = std::make_unique<Directory>();
    sub->add(std::make_unique<File>(4));
    sub->add(std::make_unique<File>(5));
    root.add(std::move(sub));
    assert(root.size() == 12);
    return 0;
}