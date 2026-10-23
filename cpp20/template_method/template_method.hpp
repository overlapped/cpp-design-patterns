#pragma once
#include <concepts>

template<typename T>
concept StepProvider = requires(T t) {
    { t.step1() } -> std::same_as<void>;
    { t.step2() } -> std::same_as<void>;
    { t.step3() } -> std::same_as<void>;
};

template<typename T>
class AbstractClass {
public:
    void templateMethod() {
        static_assert(StepProvider<T>, "T must implement step1/step2/step3");
        static_cast<T*>(this)->step1();
        static_cast<T*>(this)->step2();
        static_cast<T*>(this)->step3();
    }
};

class ConcreteClass : public AbstractClass<ConcreteClass> {
public:
    void step1() {}
    void step2() {}
    void step3() {}
};