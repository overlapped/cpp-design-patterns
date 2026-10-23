#include <gtest/gtest.h>
#include "composite.hpp"

TEST(CompositeTest, SumsRecursively) {
    Directory root;
    root.add(std::unique_ptr<Node>(new File(1)));
    auto d = std::unique_ptr<Directory>(new Directory());
    d->add(std::unique_ptr<Node>(new File(2)));
    d->add(std::unique_ptr<Node>(new File(3)));
    root.add(std::move(d));
    EXPECT_EQ(root.size(), 6);
}