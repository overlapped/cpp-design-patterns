#include <gtest/gtest.h>
#include "kiss.hpp"

TEST(KissTest, SimpleVersionMatchesComplex) {
    std::vector<int> v{1, 2, 3, 4, 5};
    auto* p = bad::ProcessorFactory::create();
    int complex_result = p->process(v);
    delete p;
    EXPECT_EQ(good::sum(v), complex_result);
}

TEST(KissTest, EmptyVector) {
    EXPECT_EQ(good::sum({}), 0);
}