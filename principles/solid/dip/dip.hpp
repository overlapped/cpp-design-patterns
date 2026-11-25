#pragma once
#include <string>
#include <memory>

// ❌ Плохо: высокоуровневый класс жёстко зависит от конкретного логгера
namespace bad {
    class FileLogger {
    public:
        void log(const std::string& msg) { /* пишет в файл */ (void)msg; }
    };

    class Service {
    public:
        void doWork() { logger_.log("work"); }
    private:
        FileLogger logger_;   // жёсткая зависимость
    };
}

// ✅ Хорошо: зависимость от абстракции ILogger
namespace good {
    class ILogger {
    public:
        virtual ~ILogger() = default;
        virtual void log(const std::string& msg) = 0;
    };

    class FileLogger : public ILogger {
    public:
        void log(const std::string&) override {}
    };

    class ConsoleLogger : public ILogger {
    public:
        void log(const std::string&) override {}
    };

    class Service {
    public:
        explicit Service(ILogger& logger) : logger_(logger) {}
        void doWork() { logger_.log("work"); }
    private:
        ILogger& logger_;
    };
}