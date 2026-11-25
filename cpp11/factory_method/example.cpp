#include "factory_method.hpp"
#include <cassert>

int main() {
    PdfCreator pc;
    auto d = pc.create();
    assert(d->type() == "PDF");
    return 0;
}