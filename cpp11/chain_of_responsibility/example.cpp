#include "chain_of_responsibility.hpp"
#include <string>
#include <cassert>

class Logger : public Handler {
public:
    std::string log;
    void handle(int request) override {
        log += std::to_string(request);
        Handler::handle(request);
    }
};

int main() {
    Logger a, b;
    a.setNext(&b);
    a.handle(42);
    assert(a.log == "42");
    assert(b.log == "42");
    return 0;
}