#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

// ❌ Плохо: одна функция делает всё — парсит, валидирует, считает, печатает
namespace bad {
    inline void processCsvLine(const std::string& line) {
        std::vector<int> nums;
        std::stringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, ',')) {
            nums.push_back(std::stoi(tok));        // парсинг + может бросить
        }
        int sum = 0;
        for (int n : nums) {
            if (n < 0) throw std::runtime_error("negative");  // валидация
            sum += n;                                          // бизнес-логика
        }
        std::printf("sum=%d\n", sum);               // вывод
    }
}

// ✅ Хорошо: каждая функция — одна забота
namespace good {
    // 1. Парсинг: строка -> вектор чисел
    inline std::vector<int> parseCsv(const std::string& line) {
        std::vector<int> nums;
        std::stringstream ss(line);
        std::string tok;
        while (std::getline(ss, tok, ',')) {
            nums.push_back(std::stoi(tok));
        }
        return nums;
    }

    // 2. Валидация: отдельно от парсинга
    inline void validateNonNegative(const std::vector<int>& nums) {
        for (int n : nums)
            if (n < 0)
                throw std::runtime_error("negative value not allowed");
    }

    // 3. Бизнес-логика: чистая функция
    inline int sum(const std::vector<int>& nums) {
        int s = 0;
        for (int n : nums) s += n;
        return s;
    }

    // 4. Вывод: отдельно, чтобы можно было заменить (файл, лог, тест)
    inline std::string formatSum(int s) {
        std::ostringstream os;
        os << "sum=" << s;
        return os.str();
    }

    // Композиция забот — на уровне вызывающего кода
    inline std::string processCsvLine(const std::string& line) {
        auto nums = parseCsv(line);
        validateNonNegative(nums);
        return formatSum(sum(nums));
    }
}