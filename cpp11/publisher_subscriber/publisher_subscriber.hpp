#pragma once
#include <functional>
#include <map>
#include <vector>

template<typename... Args>
class Publisher {
public:
    using Handler = std::function<void(Args...)>;
    using Token = std::size_t;

    Token subscribe(Handler h) {
        Token t = next_token_++;
        handlers_[t] = std::move(h);
        return t;
    }

    void unsubscribe(Token t) { handlers_.erase(t); }

    void publish(Args... args) {
        for (auto& pair : handlers_) {
            pair.second(args...);
        }
    }

private:
    std::map<Token, Handler> handlers_;
    Token next_token_ = 0;
};