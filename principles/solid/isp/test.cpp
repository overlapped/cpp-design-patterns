#include <gtest/gtest.h>
#include "isp.hpp"
#include <type_traits>

TEST(IspTest, RobotIsNotFeedable) {
    static_assert(std::is_base_of_v<good::IWorkable, good::Robot>);
    static_assert(!std::is_base_of_v<good::IFeedable, good::Robot>);
    SUCCEED();
}

TEST(IspTest, HumanIsBoth) {
    static_assert(std::is_base_of_v<good::IWorkable, good::Human>);
    static_assert(std::is_base_of_v<good::IFeedable, good::Human>);
    SUCCEED();
}