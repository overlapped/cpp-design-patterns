#pragma once
#include <string>

// ❌ Плохо: «толстый» интерфейс — робот вынужден реализовывать eat()
namespace bad {
    class IWorker {
    public:
        virtual ~IWorker() = default;
        virtual void work() = 0;
        virtual void eat()  = 0;
    };

    class Human : public IWorker {
    public:
        void work() override {}
        void eat()  override {}
    };

    class Robot : public IWorker {
    public:
        void work() override {}
        void eat()  override { /* робот не ест — вынужденная заглушка */ }
    };
}

// ✅ Хорошо: узкие интерфейсы
namespace good {
    class IWorkable {
    public:
        virtual ~IWorkable() = default;
        virtual void work() = 0;
    };

    class IFeedable {
    public:
        virtual ~IFeedable() = default;
        virtual void eat() = 0;
    };

    class Human : public IWorkable, public IFeedable {
    public:
        void work() override {}
        void eat()  override {}
    };

    class Robot : public IWorkable {
    public:
        void work() override {}
        // eat() не нужен — и не навязывается
    };
}