#include <gtest/gtest.h>
#include "adapter.hpp"

TEST(AdapterTest, AdaptsLegacyInterface) {
    LegacyPrinter legacy;
    LegacyPrinterAdapter a(legacy);
    EXPECT_EQ(a.print(), "legacy");
}