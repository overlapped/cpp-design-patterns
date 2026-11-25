#include <gtest/gtest.h>
#include "composition_over_inheritance.hpp"

TEST(CompositionTest, NotifierDelegatesToLogger) {
    good::Logger logger;
    good::EmailNotifier n(logger);
    EXPECT_NO_THROW(n.send("bob@example.com"));
}

TEST(CompositionTest, SameLoggerSharedByTwoNotifiers) {
    good::Logger logger;
    good::EmailNotifier email(logger);
    good::SmsNotifier   sms(logger);
    EXPECT_NO_THROW(email.send("a@b.c"));
    EXPECT_NO_THROW(sms.send("+000"));
}