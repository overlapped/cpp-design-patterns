#include "state.hpp"
#include <cassert>

int main() {
    Context ctx;
    ctx.setState(std::unique_ptr<State>(new ConcreteStateA()));
    ctx.request();  // A -> B
    ctx.request();  // B -> A
    return 0;
}