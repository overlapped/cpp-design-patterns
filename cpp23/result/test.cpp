#include <gtest/gtest.h>
#include "result.hpp"

TEST(ResultTest, SuccessPipeline) {
    auto r = process_age("42");
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r.value(), "User is 42 years old");
}

TEST(ResultTest, ParseError) {
    auto r = process_age("not_a_number");
    ASSERT_FALSE(r.has_value());
    EXPECT_NE(r.error().find("invalid number"), std::string::npos);
}

TEST(ResultTest, ValidationError) {
    auto r = process_age("200");
    ASSERT_FALSE(r.has_value());
    EXPECT_NE(r.error().find("age out of range"), std::string::npos);
}

TEST(ResultTest, OrElseRecovery) {
    auto r = parse_age("bad").or_else([](const Error&) -> Result<int> {
        return 0;  // recovery
    });
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r.value(), 0);
}

TEST(ResultTest, TransformMapsValue) {
    auto r = parse_age("10").transform([](int age) {
        return age * 2;
    });
    ASSERT_TRUE(r.has_value());
    EXPECT_EQ(r.value(), 20);
}