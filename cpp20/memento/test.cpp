#include <gtest/gtest.h>
#include "memento.hpp"

TEST(MementoTest, RestoresState) {
    Editor e;
    History h;
    e.type("v1");
    h.push(e.save());
    e.type("-v2");
    h.undo(e);
    EXPECT_EQ(e.text(), "v1");
}