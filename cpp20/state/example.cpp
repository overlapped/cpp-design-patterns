#include "state.hpp"
#include <cassert>

int main() {
    Context ctx;
    ctx.setState(std::make_unique<ConcreteStateA>());
    ctx.request();
    ctx.request();
    return 0;
}