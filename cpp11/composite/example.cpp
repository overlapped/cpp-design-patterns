#include "composite.hpp"
#include <cassert>

int main() {
    Directory root;
    root.add(std::unique_ptr<Node>(new File(3)));
    auto sub = std::unique_ptr<Directory>(new Directory());
    sub->add(std::unique_ptr<Node>(new File(4)));
    sub->add(std::unique_ptr<Node>(new File(5)));
    root.add(std::move(sub));
    assert(root.size() == 12);
    return 0;
}