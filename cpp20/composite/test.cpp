#include <gtest/gtest.h>
#include "composite.hpp"

TEST(CompositeTest, SumsRecursively) {
    Directory root;
    root.add(std::make_unique<File>(1));
    auto d = std::make_unique<Directory>();
    d->add(std::make_unique<File>(2));
    d->add(std::make_unique<File>(3));
    root.add(std::move(d));
    EXPECT_EQ(root.size(), 6);
}