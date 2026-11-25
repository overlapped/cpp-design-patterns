#include <gtest/gtest.h>
#include "template_method.hpp"
#include <vector>

class Tracking : public AbstractClass {
public:
    std::vector<int> order;
protected:
    void step1() override { order.push_back(1); }
    void step2() override { order.push_back(2); }
    void step3() override { order.push_back(3); }
};

TEST(TemplateMethodTest, CallsStepsInOrder) {
    Tracking t;
    t.templateMethod();
    EXPECT_EQ(t.order, (std::vector<int>{1, 2, 3}));
}