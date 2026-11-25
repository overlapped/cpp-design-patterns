#include <gtest/gtest.h>
#include "facade.hpp"

TEST(FacadeTest, Works) {
    Facade f;
    EXPECT_NO_THROW(f.operation());
}