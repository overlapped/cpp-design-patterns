#include <gtest/gtest.h>
#include "watchdog.hpp"
#include <chrono>

using namespace std::chrono;
using TP = steady_clock::time_point;
using Ms = milliseconds;

TEST(WatchdogTest, NotStartedNeverExpires) {
    Watchdog<steady_clock> wd(Ms(10));
    EXPECT_FALSE(wd.started());
    EXPECT_FALSE(wd.expired(TP{Ms(1000)}));
}

TEST(WatchdogTest, ExpiresAfterTimeout) {
    Watchdog<steady_clock> wd(Ms(100));
    TP t0{Ms(0)};
    wd.start(t0);
    EXPECT_FALSE(wd.expired(t0 + Ms(99)));
    EXPECT_TRUE(wd.expired(t0 + Ms(100)));
    EXPECT_TRUE(wd.expired(t0 + Ms(101)));
}

TEST(WatchdogTest, KickResetsDeadline) {
    Watchdog<steady_clock> wd(Ms(50));
    TP t0{Ms(0)};
    wd.start(t0);
    wd.kick(t0 + Ms(40));
    EXPECT_FALSE(wd.expired(t0 + Ms(50)));
    EXPECT_TRUE(wd.expired(t0 + Ms(90)));
}

TEST(WatchdogTest, ExpiredIsSticky) {
    Watchdog<steady_clock> wd(Ms(10));
    TP t0{Ms(0)};
    wd.start(t0);
    EXPECT_TRUE(wd.expired(t0 + Ms(20)));
    // Даже если «время пошло назад», флаг остаётся
    EXPECT_TRUE(wd.expired(t0 + Ms(5)));
}

TEST(WatchdogTest, RestartAfterExpiry) {
    Watchdog<steady_clock> wd(Ms(10));
    TP t0{Ms(0)};
    wd.start(t0);
    EXPECT_TRUE(wd.expired(t0 + Ms(20)));
    wd.start(t0 + Ms(30));
    EXPECT_FALSE(wd.expired(t0 + Ms(35)));
}

TEST(WatchdogTest, RemainingReturnsZeroWhenExpired) {
    Watchdog<steady_clock> wd(Ms(10));
    TP t0{Ms(0)};
    wd.start(t0);
    EXPECT_EQ(wd.remaining(t0 + Ms(4)), Ms(6));
    EXPECT_EQ(wd.remaining(t0 + Ms(20)), Ms(0));
}