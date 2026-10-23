#include "adapter.hpp"
#include <cassert>
#include <string_view>

int main() {
    LegacyPrinter legacy;
    LegacyPrinterAdapter adapter(legacy);
    assert(adapter.print() == std::string_view("legacy"));
    return 0;
}