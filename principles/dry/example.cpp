#include "dry.hpp"
#include <cassert>
#include <iostream>

struct Person { std::string name; int age; };

int main() {
    assert(bad::formatUser("Alice", 30) == good::formatUser("Alice", 30));
    assert(bad::formatAdmin("Bob", 40) == good::formatAdmin("Bob", 40));

#if __cplusplus >= 202002L
    Person p{"Carol", 25};
    assert(modern::format(p) == good::formatUser("Carol", 25));
#endif

    std::cout << "DRY: duplicated and shared versions agree\n";
    return 0;
}