#include <gtest/gtest.h>
#include "di_container.hpp"
#include <string>

struct IFoo {
    virtual ~IFoo() = default;
    virtual int value() const = 0;
};

struct FooImpl : IFoo {
    int v;
    explicit FooImpl(int v) : v(v) {}
    int value() const override { return v; }
};

TEST(DIContainerTest, ResolveFactory) {
    DIContainer c;
    c.registerFactory<IFoo>([] { return std::make_shared<FooImpl>(42); });
    auto foo = c.resolve<IFoo>();
    ASSERT_NE(foo, nullptr);
    EXPECT_EQ(foo->value(), 42);
}

TEST(DIContainerTest, ResolveIsSingleton) {
    DIContainer c;
    c.registerFactory<IFoo>([] { return std::make_shared<FooImpl>(1); });
    auto a = c.resolve<IFoo>();
    auto b = c.resolve<IFoo>();
    EXPECT_EQ(a.get(), b.get());
}

TEST(DIContainerTest, RegisterInstance) {
    DIContainer c;
    auto inst = std::make_shared<FooImpl>(7);
    c.registerInstance<IFoo>(inst);
    auto resolved = c.resolve<IFoo>();
    EXPECT_EQ(resolved.get(), inst.get());
    EXPECT_EQ(resolved->value(), 7);
}

TEST(DIContainerTest, UnregisteredThrows) {
    DIContainer c;
    EXPECT_THROW(c.resolve<IFoo>(), std::runtime_error);
}

TEST(DIContainerTest, IsRegistered) {
    DIContainer c;
    EXPECT_FALSE(c.isRegistered<IFoo>());
    c.registerFactory<IFoo>([] { return std::make_shared<FooImpl>(0); });
    EXPECT_TRUE(c.isRegistered<IFoo>());
}

TEST(DIContainerTest, ClearRemovesRegistrations) {
    DIContainer c;
    c.registerFactory<IFoo>([] { return std::make_shared<FooImpl>(0); });
    EXPECT_TRUE(c.isRegistered<IFoo>());
    c.clear();
    EXPECT_FALSE(c.isRegistered<IFoo>());
    EXPECT_THROW(c.resolve<IFoo>(), std::runtime_error);
}

TEST(DIContainerTest, RegisterTypeHelper) {
    DIContainer c;
    registerType<IFoo, FooImpl>(c, 99);
    auto foo = c.resolve<IFoo>();
    ASSERT_NE(foo, nullptr);
    EXPECT_EQ(foo->value(), 99);
}