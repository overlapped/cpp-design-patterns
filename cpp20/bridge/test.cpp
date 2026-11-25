#include <gtest/gtest.h>
#include "bridge.hpp"
#include <memory>

class CountingImpl : public Implementor {
public:
    int calls = 0;
    void operationImpl() override { ++calls; }
};

TEST(BridgeTest, OwnsImpl) {
    auto impl = std::make_unique<CountingImpl>();
    auto* raw = impl.get();
    Abstraction abs(std::move(impl));
    abs.operation();
    EXPECT_EQ(raw->calls, 1);
}