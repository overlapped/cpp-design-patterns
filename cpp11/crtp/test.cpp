#include <gtest/gtest.h>
#include "crtp.hpp"

class Counter : public Base<Counter> {
public:
    int count = 0;
    void implementation() { ++count; }
};

TEST(CrtpTest, CallsDerivedImpl) {
    Counter c;
    c.interface();
    EXPECT_EQ(c.count, 1);
}