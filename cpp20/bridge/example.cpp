#include "bridge.hpp"
#include <cassert>
#include <memory>

class CountingImpl : public Implementor {
public:
    int calls = 0;
    void operationImpl() override { ++calls; }
};

int main() {
    auto impl = std::make_unique<CountingImpl>();
    auto* raw = impl.get();
    Abstraction abs(std::move(impl));
    abs.operation();
    assert(raw->calls == 1);
    return 0;
}