#include <gtest/gtest.h>
#include "chain_of_responsibility.hpp"
#include <memory>

class Recording : public Handler {
public:
    int last = -1;
    void handle(int r) override {
        last = r;
        Handler::handle(r);
    }
};

TEST(ChainTest, UniquePtrChain) {
    auto a = std::make_unique<Recording>();
    auto b = std::make_unique<Recording>();
    auto* ra = a.get();
    auto* rb = b.get();
    a->setNext(std::move(b));
    a->handle(7);
    EXPECT_EQ(ra->last, 7);
    EXPECT_EQ(rb->last, 7);
}