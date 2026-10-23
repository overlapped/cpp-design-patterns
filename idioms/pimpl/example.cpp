#include "pimpl.hpp"
#include <cassert>
#include <iostream>

int main() {
    HttpClient c("https://api.example.com");
    std::string r1 = c.get("/users");
    assert(r1 == "GET https://api.example.com/users");
    // Повторный запрос — из кеша, результат тот же
    std::string r2 = c.get("/users");
    assert(r1 == r2);

    // Копирование
    HttpClient copy = c;
    assert(copy.get("/users") == r1);

    std::cout << "PImpl example OK\n";
    return 0;
}