#include <gtest/gtest.h>
#include "raii.hpp"
#include <cstdio>
#include <fstream>
#include <mutex>
#include <stdexcept>
#include <string>

TEST(RaiiTest, FileIsClosedAfterScope) {
    const std::string path = "/tmp/raii_test_file.txt";
    {
        FileHandle f(path);
        f.write("data");
        EXPECT_TRUE(f.isOpen());
    }
    // Файл должен существовать и содержать "data"
    std::ifstream in(path);
    std::string content;
    in >> content;
    EXPECT_EQ(content, "data");
    std::remove(path.c_str());
}

TEST(RaiiTest, MoveTransfersOwnership) {
    const std::string path = "/tmp/raii_test_move.txt";
    FileHandle a(path);
    FileHandle b(std::move(a));
    EXPECT_FALSE(a.isOpen());
    EXPECT_TRUE(b.isOpen());
    std::remove(path.c_str());
}

TEST(RaiiTest, LockReleasedOnException) {
    std::mutex m;
    try {
        ScopedLock lock(m);
        throw std::runtime_error("x");
    } catch (...) {}
    EXPECT_TRUE(m.try_lock());
    m.unlock();
}

TEST(RaiiTest, ThrowOnMissingFile) {
    EXPECT_THROW(FileHandle("/nonexistent_dir/xxx/file.txt"), std::runtime_error);
}