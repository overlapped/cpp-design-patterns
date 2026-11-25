#include <gtest/gtest.h>
#include "srp.hpp"

TEST(SrpTest, JsonMatches) {
    bad::User  bu("Bob", 40);
    good::User gu{"Bob", 40};
    EXPECT_EQ(bu.toJson(), good::toJson(gu));
}

TEST(SrpTest, RepositorySaves) {
    good::UserRepository repo("srp_test_tmp.json");
    repo.save({"Carol", 25});
    std::ifstream f("srp_test_tmp.json");
    std::string s((std::istreambuf_iterator<char>(f)), {});
    EXPECT_NE(s.find("Carol"), std::string::npos);
    std::remove("srp_test_tmp.json");
}