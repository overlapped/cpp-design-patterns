#include "type_erasure.hpp"

#include <gtest/gtest.h>

#include <string>
#include <utility>
#include <vector>

namespace {

struct SmallSquare {
    double side;

    [[nodiscard]] std::string draw() const { return "Square"; }
    [[nodiscard]] double area() const { return side * side; }
};

struct HeavyMesh {
    double matrix[16]{}; // 128 байт — гарантированно больше kSboSize (32 байта)
    double total_area{100.0};

    [[nodiscard]] std::string draw() const { return "HeavyMesh"; }
    [[nodiscard]] double area() const { return total_area; }
};

} // namespace

TEST(TypeErasureTest, StoresHeterogeneousTypesByValue) {
    std::vector<idioms::Shape> shapes;
    shapes.emplace_back(SmallSquare{4.0});
    shapes.emplace_back(HeavyMesh{{}, 250.0});

    EXPECT_EQ(shapes[0].draw(), "Square");
    EXPECT_DOUBLE_EQ(shapes[0].area(), 16.0);

    EXPECT_EQ(shapes[1].draw(), "HeavyMesh");
    EXPECT_DOUBLE_EQ(shapes[1].area(), 250.0);
}

TEST(TypeErasureTest, SmallBufferOptimizationAvoidsHeapForSmallTypes) {
    idioms::Shape small = SmallSquare{3.0};
    idioms::Shape heavy = HeavyMesh{};

    EXPECT_FALSE(small.is_stored_on_heap());
    EXPECT_TRUE(heavy.is_stored_on_heap());
}

TEST(TypeErasureTest, ValueSemanticsCopyAndMove) {
    idioms::Shape original = SmallSquare{5.0};
    idioms::Shape copied = original;

    EXPECT_EQ(copied.draw(), "Square");
    EXPECT_DOUBLE_EQ(copied.area(), 25.0);
    EXPECT_FALSE(copied.is_stored_on_heap());

    // Проверка копирования и перемещения для heap-объекта
    idioms::Shape heavy_orig = HeavyMesh{{}, 77.0};
    idioms::Shape heavy_copy = heavy_orig;
    EXPECT_DOUBLE_EQ(heavy_copy.area(), 77.0);
    EXPECT_TRUE(heavy_copy.is_stored_on_heap());

    idioms::Shape heavy_moved = std::move(heavy_copy);
    EXPECT_DOUBLE_EQ(heavy_moved.area(), 77.0);
    EXPECT_TRUE(heavy_moved.is_stored_on_heap());
}

TEST(TypeErasureTest, ConceptRejectsIncompatibleTypes) {
    struct NotAShape {
        void render() const {}
    };

    static_assert(idioms::DrawableLike<SmallSquare>);
    static_assert(idioms::DrawableLike<HeavyMesh>);
    static_assert(!idioms::DrawableLike<NotAShape>);
}
