#pragma once
#include <memory>
#include <string>
#include <iostream>

// ❌ Плохо: наследование ради переиспользования кода
// Класс EmailNotifier «является» Logger'ом — это ложная связь
namespace bad {
    class Logger {
    public:
        void log(const std::string& msg) const {
            std::cout << "[log] " << msg << "\n";
        }
    };

    class EmailNotifier : public Logger {   // ← наследование только чтобы звать log()
    public:
        void send(const std::string& to) const {
            log("sending email to " + to);  // используем унаследованный метод
        }
    };
}

// ✅ Хорошо: композиция — Notifier содержит Logger
namespace good {
    class Logger {
    public:
        void log(const std::string& msg) const {
            std::cout << "[log] " << msg << "\n";
        }
    };

    class EmailNotifier {
    public:
        explicit EmailNotifier(Logger& logger) : logger_(logger) {}
        void send(const std::string& to) const {
            logger_.log("sending email to " + to);
        }
    private:
        Logger& logger_;
    };

    // Бонус: можно легко подменить логгер (в т.ч. в тестах)
    class SmsNotifier {
    public:
        explicit SmsNotifier(Logger& logger) : logger_(logger) {}
        void send(const std::string& to) const {
            logger_.log("sending sms to " + to);
        }
    private:
        Logger& logger_;
    };
}