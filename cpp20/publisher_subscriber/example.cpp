#include "publisher_subscriber.hpp"
#include <cassert>
#include <string>

int main() {
    Publisher<std::string> pub;
    std::string got;
    pub.subscribe([&](std::string s) { got = std::move(s); });
    pub.publish("hello");
    assert(got == "hello");
    return 0;
}