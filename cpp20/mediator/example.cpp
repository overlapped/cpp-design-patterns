#include "mediator.hpp"
#include <cassert>

int main() {
    ChatMediator chat;
    User a(&chat), b(&chat);
    chat.add(&a);
    chat.add(&b);
    a.send("hi");
    assert(b.inbox.size() == 1);
    return 0;
}