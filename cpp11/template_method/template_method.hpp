#pragma once

class AbstractClass {
public:
    virtual ~AbstractClass() = default;
    void templateMethod() {
        step1();
        step2();
        step3();
    }
protected:
    virtual void step1() = 0;
    virtual void step2() = 0;
    virtual void step3() = 0;
};

class ConcreteClass : public AbstractClass {
protected:
    void step1() override {}
    void step2() override {}
    void step3() override {}
};