#include "isp.hpp"
#include <iostream>

int main() {
    good::Human h;
    good::Robot r;
    h.work(); h.eat();
    r.work();               // r.eat() не существует — и это правильно
    std::cout << "ISP: Robot only implements what it needs\n";
    return 0;
}