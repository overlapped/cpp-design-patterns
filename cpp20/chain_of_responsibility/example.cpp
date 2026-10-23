#include "chain_of_responsibility.hpp"
#include <cassert>
#include <memory>
#include <vector>

class Recording : public Handler {
public:
    int last = -1;
    void handle(int r) override {
        last = r;
        Handler::handle(r);
    }
};

int main() {
    auto a = std::make_unique<Recording>();
    auto b = std::make_unique<Recording>();
    auto* ra = a.get();
    auto* rb = b.get();
    a->setNext(std::move(b));
    a->handle(7);
    assert(ra->last == 7 && rb->last == 7);
    return 0;
}