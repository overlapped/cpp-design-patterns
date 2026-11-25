#include "memento.hpp"
#include <cassert>

int main() {
    Editor e;
    History h;
    e.type("abc");
    h.push(e.save());
    e.type("def");
    h.undo(e);
    assert(e.text() == "abc");
    return 0;
}