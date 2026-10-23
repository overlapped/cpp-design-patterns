#include "bridge.hpp"
#include <string>
#include <cassert>

class StringImpl : public Implementor {
public:
    std::string result;
    void operationImpl() override { result = "impl"; }
};

int main() {
    StringImpl impl;
    Abstraction abs(&impl);
    abs.operation();
    assert(impl.result == "impl");
    return 0;
}