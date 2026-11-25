#include <gtest/gtest.h>
#include "template_method.hpp"

class Counter : public AbstractClass<Counter> {
public:
    int count = 0;
    void step1() { ++count; }
    void step2() { ++count; }
    void step3() { ++count; }
};

TEST(TemplateMethodTest, ConceptBased) {
    Counter c;
    c.templateMethod();
    EXPECT_EQ(c.count, 3);
}