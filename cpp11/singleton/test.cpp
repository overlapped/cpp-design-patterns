#include <gtest/gtest.h>
#include "singleton.hpp"

TEST(SingletonTest, SameInstance) {
    EXPECT_EQ(&Singleton::instance(), &Singleton::instance());
}