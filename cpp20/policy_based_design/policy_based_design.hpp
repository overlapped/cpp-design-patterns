#pragma once
#include <concepts>
#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <memory>

// Policy-Based Design: поведение класса parameterizуется
// шаблонными политиками, а не наследованием. Каждая политика —
// отдельный аспект (логирование, хранение, проверка), и их можно
// комбинировать свободно.
//
// В C++20 политики проверяются через concepts: если политика не
// удовлетворяет контракту, компилятор скажет об этом явно, а не
// в глубине шаблонных ошибок.

// --- Политики логирования ---
struct ConsoleLogger {
    void log(const std::string& msg) const {
        std::cout << "[console] " << msg << "\n";
    }
};

struct SilentLogger {
    void log(const std::string&) const {}
};

// Concept: политика логирования должна иметь log(string)
template<typename T>
concept LoggerPolicy = requires(const T& t, const std::string& s) {
    { t.log(s) } -> std::same_as<void>;
};

// --- Политики хранения ---
template<typename T>
struct VectorStorage {
    std::vector<T> data;
    void add(const T& v) { data.push_back(v); }
    std::size_t size() const { return data.size(); }
    const T& get(std::size_t i) const { return data[i]; }
};

template<typename T>
struct ArrayStorage {
    static constexpr std::size_t Capacity = 16;
    T data[Capacity];
    std::size_t count = 0;
    void add(const T& v) {
        if (count < Capacity) data[count++] = v;
    }
    std::size_t size() const { return count; }
    const T& get(std::size_t i) const { return data[i]; }
};

// Concept: политика хранения должна иметь add, size, get
template<typename S, typename T>
concept StoragePolicy = requires(S s, const S cs, const T& v, std::size_t i) {
    { s.add(v) } -> std::same_as<void>;
    { cs.size() } -> std::convertible_to<std::size_t>;
    { cs.get(i) } -> std::convertible_to<const T&>;
};

// --- Политики проверки ---
template<typename T>
struct NoCheck {
    bool operator()(const T&) const { return true; }
};

template<typename T>
struct PositiveCheck {
    bool operator()(const T& v) const { return v > T{}; }
};

// --- Класс с политиками ---
template<typename T,
         LoggerPolicy Logger = ConsoleLogger,
         template<typename> typename Storage = VectorStorage,
         typename Check = NoCheck<T>>
class Repository {
public:
    void add(const T& v) {
        if (!check_(v)) {
            logger_.log("rejected: " + std::to_string(v));
            return;
        }
        storage_.add(v);
        logger_.log("added: " + std::to_string(v));
    }

    std::size_t size() const { return storage_.size(); }
    const T& get(std::size_t i) const { return storage_.get(i); }

private:
    Logger logger_;
    Storage<T> storage_;
    Check check_;
};