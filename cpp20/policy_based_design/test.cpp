#include <gtest/gtest.h>
#include "policy_based_design.hpp"

TEST(PolicyBasedDesignTest, DefaultPolicies) {
    Repository<int> r;
    r.add(1);
    r.add(2);
    EXPECT_EQ(r.size(), 2u);
}

TEST(PolicyBasedDesignTest, SilentLoggerNoOutput) {
    Repository<int, SilentLogger> r;
    r.add(1);
    EXPECT_EQ(r.size(), 1u);
}

TEST(PolicyBasedDesignTest, ArrayStorageCapacity) {
    Repository<int, SilentLogger, ArrayStorage> r;
    for (int i = 0; i < 20; ++i) r.add(i);
    // ArrayStorage вмещает только 16
    EXPECT_EQ(r.size(), 16u);
}

TEST(PolicyBasedDesignTest, PositiveCheckRejectsNegative) {
    Repository<int, SilentLogger, VectorStorage, PositiveCheck<int>> r;
    r.add(10);
    r.add(-5);
    r.add(0);
    r.add(20);
    EXPECT_EQ(r.size(), 2u);
    EXPECT_EQ(r.get(0), 10);
    EXPECT_EQ(r.get(1), 20);
}

TEST(PolicyBasedDesignTest, CombinedPolicies) {
    Repository<int, SilentLogger, ArrayStorage, PositiveCheck<int>> r;
    r.add(1);
    r.add(-1);
    r.add(2);
    r.add(0);
    r.add(3);
    EXPECT_EQ(r.size(), 3u);
    EXPECT_EQ(r.get(0), 1);
    EXPECT_EQ(r.get(1), 2);
    EXPECT_EQ(r.get(2), 3);
}

TEST(PolicyBasedDesignTest, ConceptChecksLogger) {
    // ConsoleLogger и SilentLogger удовлетворяют LoggerPolicy
    static_assert(LoggerPolicy<ConsoleLogger>);
    static_assert(LoggerPolicy<SilentLogger>);
    SUCCEED();
}

TEST(PolicyBasedDesignTest, ConceptChecksStorage) {
    static_assert(StoragePolicy<VectorStorage<int>, int>);
    static_assert(StoragePolicy<ArrayStorage<int>, int>);
    SUCCEED();
}