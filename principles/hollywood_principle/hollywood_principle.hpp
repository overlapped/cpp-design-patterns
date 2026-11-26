#pragma once
#include <functional>
#include <string>
#include <vector>
#include <iostream>

// Hollywood Principle: «Не звоните нам, мы позвоним вам».
// Фреймворк (или библиотека) сам вызывает ваш код в нужный момент,
// а не наоборот. Это инверсия управления (IoC) в её классическом
// понимании: не клиент дёргает методы фреймворка, а фреймворк
// вызывает клиентские callback'и.

// ❌ Плохо: клиент сам управляет потоком — сам решает, когда
// вызывать обработчики, в каком порядке, сколько раз.
namespace bad {
    class EventSource {
    public:
        void addHandler(std::function<void(const std::string&)> h) {
            handlers_.push_back(std::move(h));
        }

        // Клиент должен сам вызвать fire() — фреймворк не управляет
        // потоком. Легко забыть, легко вызвать не в тот момент.
        void fire(const std::string& event) {
            for (auto& h : handlers_) h(event);
        }

    private:
        std::vector<std::function<void(const std::string&)>> handlers_;
    };
}

// ✅ Хорошо: фреймворк управляет потоком. Клиент только регистрирует
// обработчик, а фреймворк сам решает, когда его вызвать (например,
// по таймеру, по событию, по завершении асинхронной операции).
namespace good {
    class EventLoop {
    public:
        using Handler = std::function<void(const std::string&)>;

        // Клиент регистрирует обработчик и больше ничего не делает.
        void onEvent(Handler h) {
            handler_ = std::move(h);
        }

        // Фреймворк сам «звонит» клиенту. Клиент не вызывает это
        // напрямую — это делает run().
        void run(const std::vector<std::string>& events) {
            for (const auto& e : events) {
                if (handler_) handler_(e);
            }
        }

    private:
        Handler handler_;
    };

    // Второй пример: шаблонный метод — фреймворк вызывает шаги
    // в фиксированном порядке, клиент переопределяет только шаги.
    class Pipeline {
    public:
        virtual ~Pipeline() = default;

        // Фреймворк управляет порядком вызовов
        void execute() {
            step1();
            step2();
            step3();
        }

    protected:
        virtual void step1() = 0;
        virtual void step2() = 0;
        virtual void step3() = 0;
    };
}