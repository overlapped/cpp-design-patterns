#include "pimpl.hpp"
#include <map>
#include <sstream>

// Здесь — «тяжёлые» зависимости и настоящее устройство класса.
// Клиенты pimpl.hpp их не видят и не тянут в свою сборку.
struct HttpClient::Impl {
    std::string base_url;
    std::map<std::string, std::string> cache;

    explicit Impl(std::string url) : base_url(std::move(url)) {}

    std::string fetch(const std::string& path) {
        auto it = cache.find(path);
        if (it != cache.end()) return it->second;
        // Имитация сетевого запроса
        std::ostringstream os;
        os << "GET " << base_url << path;
        std::string response = os.str();
        cache[path] = response;
        return response;
    }
};

HttpClient::HttpClient(std::string base_url)
    : impl_(std::make_unique<Impl>(std::move(base_url))) {}

HttpClient::~HttpClient() = default;

HttpClient::HttpClient(const HttpClient& other)
    : impl_(std::make_unique<Impl>(*other.impl_)) {}

HttpClient& HttpClient::operator=(const HttpClient& other) {
    if (this != &other) {
        *impl_ = *other.impl_;
    }
    return *this;
}

HttpClient::HttpClient(HttpClient&& other) noexcept = default;
HttpClient& HttpClient::operator=(HttpClient&& other) noexcept = default;

std::string HttpClient::get(const std::string& path) const {
    return impl_->fetch(path);
}