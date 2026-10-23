#include <gtest/gtest.h>
#include "chain_of_responsibility.hpp"

class Recording : public Handler {
public:
    int last = -1;
    void handle(int request) override {
        last = request;
        Handler::handle(request);
    }
};

TEST(ChainTest, PassesToNext) {
    Recording a, b;
    a.setNext(&b);
    a.handle(5);
    EXPECT_EQ(a.last, 5);
    EXPECT_EQ(b.last, 5);
}