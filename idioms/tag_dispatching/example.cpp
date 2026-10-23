#include "tag_dispatching.hpp"
#include <cassert>
#include <list>
#include <vector>

int main() {
    std::vector<int> v{1, 2, 3};
    std::list<int> l{4, 5, 6};

    process(v);   // fast_tag
    process(l);   // slow_tag

    assert(v[0] == 2 && v[1] == 3 && v[2] == 4);
    assert(l.front() == 5 && l.back() == 7);

    return 0;
}