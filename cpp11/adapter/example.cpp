#include "adapter.hpp"
#include <cassert>

int main() {
    LegacyPrinter legacy;
    LegacyPrinterAdapter adapter(legacy);
    assert(adapter.print() == "legacy");
    return 0;
}