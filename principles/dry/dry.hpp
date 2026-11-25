#pragma once
#include <string>
#include <vector>
#include <sstream>

// ❌ Плохо: дублирование в каждом методе
namespace bad {
    inline std::string formatUser(const std::string& name, int age) {
        std::ostringstream os;
        os << "User: " << name << " (" << age << ")";
        return os.str();
    }
    inline std::string formatAdmin(const std::string& name, int age) {
        std::ostringstream os;
        os << "User: " << name << " (" << age << ")";  // ← копипаста
        return os.str();
    }
}

// ✅ Хорошо: одна общая функция
namespace good {
    inline std::string formatPerson(const std::string& name, int age) {
        std::ostringstream os;
        os << "User: " << name << " (" << age << ")";
        return os.str();
    }
    inline std::string formatUser(const std::string& name, int age) {
        return formatPerson(name, age);
    }
    inline std::string formatAdmin(const std::string& name, int age) {
        return formatPerson(name, age);
    }
}

// ✅ Ещё лучше (C++20): шаблон + концепт для любого типа с полями name/age
#if __cplusplus >= 202002L
#include <concepts>
namespace modern {
    template<typename T>
    concept HasNameAge = requires(T t) {
        { t.name } -> std::convertible_to<std::string>;
        { t.age }  -> std::convertible_to<int>;
    };

    template<HasNameAge T>
    std::string format(const T& t) {
        std::ostringstream os;
        os << "User: " << t.name << " (" << t.age << ")";
        return os.str();
    }
}
#endif