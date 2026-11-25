#include "flyweight.hpp"
#include <cassert>

int main() {
    GlyphFactory f;
    const Glyph* a1 = f.get('a');
    const Glyph* a2 = f.get('a');
    assert(a1 == a2);
    assert(f.size() == 1);
    return 0;
}