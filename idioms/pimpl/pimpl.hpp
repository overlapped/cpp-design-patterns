#pragma once
#include <memory>
#include <string>

// Публичный заголовок НЕ включает тяжёлых зависимостей реализации.
// Клиент видит только объявление класса и его публичный API.
class HttpClient {
public:
    explicit HttpClient(std::string base_url);
    ~HttpClient();

    // Rule of Five: pimpl-класс требует явных объявлений, потому что
    // unique_ptr<Impl> в деструкторе нуждается в полном типе Impl.
    HttpClient(const HttpClient& other);
    HttpClient& operator=(const HttpClient& other);
    HttpClient(HttpClient&& other) noexcept;
    HttpClient& operator=(HttpClient&& other) noexcept;

    std::string get(const std::string& path) const;

private:
    struct Impl;               // forward declaration
    std::unique_ptr<Impl> impl_;
};