#pragma once

class SubsystemA { public: void operationA() {} };
class SubsystemB { public: void operationB() {} };

class Facade {
public:
    void operation() {
        a_.operationA();
        b_.operationB();
    }
private:
    SubsystemA a_;
    SubsystemB b_;
};