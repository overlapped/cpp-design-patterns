#include "ebo_no_unique_address.hpp"

#include <iostream>

int main() {
    std::cout << "sizeof(int*):                           " << sizeof(int*) << " bytes\n";
    std::cout << "sizeof(DefaultDeleter):                 " << sizeof(idioms::DefaultDeleter) << " byte\n\n";

    std::cout << "1. NaivePtr<int, DefaultDeleter>:       "
              << sizeof(idioms::NaivePtr<int>) << " bytes (wasted padding!)\n";

    std::cout << "2. EboPtr<int, DefaultDeleter>:         "
              << sizeof(idioms::EboPtr<int>) << " bytes (EBO optimized)\n";

    std::cout << "   EboPtr<int, FinalDeleter>:           "
              << sizeof(idioms::EboPtr<int, idioms::FinalDeleter>)
              << " bytes (EBO blocked by final)\n";

    std::cout << "3. ModernPtr<int, DefaultDeleter>:      "
              << sizeof(idioms::ModernPtr<int>) << " bytes ([[no_unique_address]])\n";

    std::cout << "   ModernPtr<int, FinalDeleter>:        "
              << sizeof(idioms::ModernPtr<int, idioms::FinalDeleter>)
              << " bytes (works even with final!)\n";

    idioms::ModernPtr<int> ptr(new int(42));
    std::cout << "\nStored value: " << *ptr.get() << '\n';

    return 0;
}
