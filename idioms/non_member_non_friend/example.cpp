#include "non_member_non_friend.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

int main() {
    Money a(100, "USD");
    Money b(50, "USD");

    Money c = a + b;
    assert(c.amount() == 150);
    assert(c == Money(150, "USD"));
    assert(a != b);

    std::ostringstream os;
    os << c;
    assert(os.str() == "150 USD");

    std::cout << "Non-Member Non-Friend example OK\n";
    return 0;
}