#include <gtest/gtest.h>
#include "pimpl.hpp"
#include <utility>

TEST(PimplTest, BasicGet) {
    HttpClient c("http://x");
    EXPECT_EQ(c.get("/a"), "GET http://x/a");
}

TEST(PimplTest, CopyIsIndependent) {
    HttpClient a("http://a");
    HttpClient b = a;
    EXPECT_EQ(b.get("/p"), "GET http://a/p");
    EXPECT_EQ(a.get("/p"), "GET http://a/p");
}

TEST(PimplTest, MoveLeavesValidSource) {
    HttpClient a("http://a");
    HttpClient b = std::move(a);
    EXPECT_EQ(b.get("/z"), "GET http://a/z");
    // Перемещённый из объекта: unique_ptr обнулён; трогать get() нельзя
    // (это UB), но сам объект можно уничтожить/присвоить заново.
    a = HttpClient("http://new");
    EXPECT_EQ(a.get("/q"), "GET http://new/q");
}