#pragma once
#include <expected>
#include <string>
#include <string_view>

// Универсальный тип ошибки для примеров
using Error = std::string;

template<typename T>
using Result = std::expected<T, Error>;

// Парсинг возраста из строки
inline Result<int> parse_age(std::string_view input) {
    try {
        std::size_t pos = 0;
        int age = std::stoi(std::string(input), &pos);
        if (pos != input.size()) {
            return std::unexpected("trailing characters: " + std::string(input));
        }
        return age;
    } catch (...) {
        return std::unexpected("invalid number: " + std::string(input));
    }
}

// Валидация возраста
inline Result<int> validate_age(int age) {
    if (age < 0 || age > 150) {
        return std::unexpected("age out of range: " + std::to_string(age));
    }
    return age;
}

// Форматирование результата
inline Result<std::string> format_age(int age) {
    return "User is " + std::to_string(age) + " years old";
}

// Pipeline: parse -> validate -> format
inline Result<std::string> process_age(std::string_view input) {
    return parse_age(input)
        .and_then(validate_age)
        .and_then(format_age);
}