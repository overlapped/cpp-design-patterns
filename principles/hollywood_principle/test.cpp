#include <gtest/gtest.h>
#include "hollywood_principle.hpp"
#include <vector>

TEST(HollywoodTest, BadVersionClientDrives) {
    bad::EventSource src;
    int calls = 0;
    src.addHandler([&](const std::string&) { ++calls; });
    src.fire("x");
    src.fire("y");
    EXPECT_EQ(calls, 2);
}

TEST(HollywoodTest, GoodVersionFrameworkDrives) {
    good::EventLoop loop;
    int calls = 0;
    loop.onEvent([&](const std::string&) { ++calls; });
    loop.run({"a", "b", "c", "d"});
    EXPECT_EQ(calls, 4);
}

TEST(HollywoodTest, NoHandlerIsSafe) {
    good::EventLoop loop;
    EXPECT_NO_THROW(loop.run({"a", "b"}));
}

class TrackingPipeline : public good::Pipeline {
public:
    std::vector<int> order;
protected:
    void step1() override { order.push_back(1); }
    void step2() override { order.push_back(2); }
    void step3() override { order.push_back(3); }
};

TEST(HollywoodTest, PipelineOrderIsFixedByFramework) {
    TrackingPipeline p;
    p.execute();
    EXPECT_EQ(p.order, (std::vector<int>{1, 2, 3}));
}

TEST(HollywoodTest, PipelineCanBeReused) {
    TrackingPipeline p;
    p.execute();
    p.execute();
    EXPECT_EQ(p.order.size(), 6u);
    EXPECT_EQ(p.order[0], 1);
    EXPECT_EQ(p.order[3], 1);
}