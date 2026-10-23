#pragma once
#include <cstdio>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>

// 1. Классический RAII: владение ресурсом через время жизни объекта.
//    Деструктор гарантированно освобождает ресурс (в т.ч. при исключении).
class FileHandle {
public:
    explicit FileHandle(const std::string& path)
        : file_(std::fopen(path.c_str(), "w")) {
        if (!file_) throw std::runtime_error("cannot open: " + path);
    }

    ~FileHandle() { close(); }

    // Копирование запрещено: два владельца одного FILE* — ошибка.
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    // Перемещение разрешено: владение передаётся, источник обнуляется.
    FileHandle(FileHandle&& other) noexcept : file_(other.file_) {
        other.file_ = nullptr;
    }
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            close();
            file_ = other.file_;
            other.file_ = nullptr;
        }
        return *this;
    }

    void write(const std::string& s) {
        if (!file_) throw std::runtime_error("file is closed");
        std::fputs(s.c_str(), file_);
    }

    bool isOpen() const { return file_ != nullptr; }

private:
    void close() noexcept {
        if (file_) {
            std::fclose(file_);
            file_ = nullptr;
        }
    }

    std::FILE* file_;
};

// 2. RAII для мьютекса — аналог std::lock_guard, чтобы показать идею «вручную».
class ScopedLock {
public:
    explicit ScopedLock(std::mutex& m) : mtx_(m) { mtx_.lock(); }
    ~ScopedLock() { mtx_.unlock(); }
    ScopedLock(const ScopedLock&) = delete;
    ScopedLock& operator=(const ScopedLock&) = delete;
private:
    std::mutex& mtx_;
};