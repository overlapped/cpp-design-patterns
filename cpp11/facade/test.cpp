#include <gtest/gtest.h>
#include "facade.hpp"

TEST(FacadeTest, CallsSubsystems) {
    Facade f;
    EXPECT_NO_THROW(f.operation());
}