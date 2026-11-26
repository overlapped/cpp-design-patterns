#include "di_container.hpp"
#include <cassert>
#include <iostream>
#include <string>

// Интерфейсы
struct ILogger {
    virtual ~ILogger() = default;
    virtual void log(const std::string&) = 0;
};

struct IDatabase {
    virtual ~IDatabase() = default;
    virtual std::string query(const std::string&) = 0;
};

// Реализации
struct ConsoleLogger : ILogger {
    void log(const std::string& msg) override {
        std::cout << "[log] " << msg << "\n";
    }
};

struct FakeDatabase : IDatabase {
    std::string query(const std::string& sql) override {
        return "result for: " + sql;
    }
};

// Сервис, который зависит от ILogger и IDatabase
class Service {
public:
    Service(std::shared_ptr<ILogger> logger, std::shared_ptr<IDatabase> db)
        : logger_(std::move(logger)), db_(std::move(db)) {}

    std::string doWork() {
        logger_->log("querying");
        return db_->query("SELECT 1");
    }

private:
    std::shared_ptr<ILogger> logger_;
    std::shared_ptr<IDatabase> db_;
};

int main() {
    DIContainer c;

    // Регистрируем реализации
    c.registerFactory<ILogger>([] { return std::make_shared<ConsoleLogger>(); });
    c.registerFactory<IDatabase>([] { return std::make_shared<FakeDatabase>(); });

    // Разрешаем зависимости
    auto logger = c.resolve<ILogger>();
    auto db = c.resolve<IDatabase>();

    Service svc(logger, db);
    std::string result = svc.doWork();
    assert(result == "result for: SELECT 1");

    // Singleton: повторный resolve возвращает тот же объект
    auto logger2 = c.resolve<ILogger>();
    assert(logger.get() == logger2.get());

    // Готовый instance
    DIContainer c2;
    auto sharedLogger = std::make_shared<ConsoleLogger>();
    c2.registerInstance<ILogger>(sharedLogger);
    auto resolved = c2.resolve<ILogger>();
    assert(resolved.get() == sharedLogger.get());

    std::cout << "DI Container: resolve dependencies by type\n";
    return 0;
}