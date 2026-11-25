#include <gtest/gtest.h>
#include "dip.hpp"

class MockLogger : public good::ILogger {
public:
    int calls = 0;
    void log(const std::string&) override { ++calls; }
};

TEST(DipTest, ServiceUsesInjectedLogger) {
    MockLogger mock;
    good::Service s(mock);
    s.doWork();
    s.doWork();
    EXPECT_EQ(mock.calls, 2);
}

TEST(DipTest, WorksWithFileLogger) {
    good::FileLogger file;
    good::Service s(file);
    EXPECT_NO_THROW(s.doWork());
}