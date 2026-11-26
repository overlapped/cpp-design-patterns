#include "ebo_no_unique_address.hpp"

#include <gtest/gtest.h>

TEST(EboAndNoUniqueAddressTest, SizeOptimizationComparison) {
    // Без оптимизации пустой делитер раздувает структуру в 2 раза (с 8 до 16 байт на 64-bit)
    EXPECT_GT(sizeof(idioms::NaivePtr<int>), sizeof(int*));

    // EBO полностью убирает накладные расходы для обычного пустого класса
    EXPECT_EQ(sizeof(idioms::EboPtr<int>), sizeof(int*));

    // Но EBO бессилен против final-классов
    EXPECT_GT(sizeof(idioms::EboPtr<int, idioms::FinalDeleter>), sizeof(int*));

    // C++20 [[no_unique_address]] оптимизирует размер в обоих случаях (включая final!)
    EXPECT_EQ(sizeof(idioms::ModernPtr<int>), sizeof(int*));
    EXPECT_EQ(sizeof(idioms::ModernPtr<int, idioms::FinalDeleter>), sizeof(int*));
}

TEST(EboAndNoUniqueAddressTest, StatefulDeleterStillAllocatesMemory) {
    struct StatefulDeleter {
        int* counter;
        void operator()(int* ptr) const noexcept {
            ++(*counter);
            delete ptr;
        }
    };

    // Если делитер хранит состояние, место под него честно выделяется
    EXPECT_GT(sizeof(idioms::ModernPtr<int, StatefulDeleter>), sizeof(int*));

    int delete_calls = 0;
    {
        idioms::ModernPtr<int, StatefulDeleter> ptr(new int(99), StatefulDeleter{&delete_calls});
        EXPECT_EQ(*ptr.get(), 99);
    }
    EXPECT_EQ(delete_calls, 1);
}

TEST(EboAndNoUniqueAddressTest, DestructorInvokesStatelessDeleter) {
    static int custom_deletes = 0;
    struct CustomDeleter {
        void operator()(int* p) const noexcept {
            ++custom_deletes;
            delete p;
        }
    };

    {
        idioms::EboPtr<int, CustomDeleter> p1(new int(10));
        idioms::ModernPtr<int, CustomDeleter> p2(new int(20));
        EXPECT_EQ(*p1.get() + *p2.get(), 30);
    }

    EXPECT_EQ(custom_deletes, 2);
}
