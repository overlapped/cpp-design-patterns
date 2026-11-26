#include "hidden_friends.hpp"

#include <iostream>
#include <sstream>

int main() {
    idioms::ModernNumber a{10};
    idioms::ModernNumber b{32};

    // Вызов проходит через ADL (Argument-Dependent Lookup)
    idioms::ModernNumber c = a + b;
    std::cout << "Sum result: " << c << '\n';

    // Поддерживается симметричное сложение с int (так как конструктор не explicit),
    // но сама функция operator+ не засоряет список перегрузок для других типов!
    idioms::ModernNumber d = 5 + a;
    std::cout << "5 + a = " << d << '\n';

    // Проверка StrictId
    idioms::StrictId id1{100};
    idioms::StrictId id2{100};
    std::cout << std::boolalpha << "id1 == id2: " << (id1 == id2) << '\n';

    // Следующая строка не скомпилируется, так как Hidden Friend не виден вне ADL:
    // auto fn_ptr = &idioms::operator+; // Error: no member named 'operator+' in namespace 'idioms'

    return 0;
}
