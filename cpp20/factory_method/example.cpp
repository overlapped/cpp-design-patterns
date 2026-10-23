#include "factory_method.hpp"
#include <cassert>

int main() {
    auto creator = makeCreator(DocType::Word);
    auto doc = creator->create();
    assert(doc->type() == "Word");
    return 0;
}