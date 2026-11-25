#include <gtest/gtest.h>
#include "tag_dispatching.hpp"
#include <list>
#include <type_traits>
#include <vector>

TEST(TagDispatchingTest, VectorUsesFastTag) {
    using tag = container_traits<std::vector<int>>::tag;
    static_assert(std::is_same_v<tag, fast_tag>, "vector should use fast_tag");
    SUCCEED();
}

TEST(TagDispatchingTest, ListUsesSlowTag) {
    using tag = container_traits<std::list<int>>::tag;
    static_assert(std::is_same_v<tag, slow_tag>, "list should use slow_tag");
    SUCCEED();
}

TEST(TagDispatchingTest, ProcessModifiesElements) {
    std::vector<int> v{1, 2, 3};
    process(v);
    EXPECT_EQ(v, (std::vector<int>{2, 3, 4}));

    std::list<int> l{4, 5};
    process(l);
    EXPECT_EQ(l.front(), 5);
    EXPECT_EQ(l.back(), 6);
}