#pragma once
#include <functional>
#include <unordered_map>
#include <vector>

template<typename... Args>
class Publisher {
public:
    using Token = std::size_t;
    using Handler = std::function<void(Args...)>;

    Token subscribe(Handler h) {
        Token t = next_++;
        handlers_[t] = std::move(h);
        return t;
    }
    void unsubscribe(Token t) { handlers_.erase(t); }
    void publish(Args... args) {
        for (auto& [_, h] : handlers_) h(args...);
    }
private:
    std::unordered_map<Token, Handler> handlers_;
    Token next_ = 0;
};