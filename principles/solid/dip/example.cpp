#include "dip.hpp"
#include <iostream>

int main() {
    good::FileLogger    file;
    good::ConsoleLogger console;

    good::Service s1(file);
    good::Service s2(console);
    s1.doWork();
    s2.doWork();

    std::cout << "DIP: Service works with any ILogger\n";
    return 0;
}