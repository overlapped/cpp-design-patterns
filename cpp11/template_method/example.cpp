#include "template_method.hpp"
#include <vector>
#include <string>
#include <cassert>

class Logger : public AbstractClass {
public:
    std::vector<std::string> log;
protected:
    void step1() override { log.push_back("step1"); }
    void step2() override { log.push_back("step2"); }
    void step3() override { log.push_back("step3"); }
};

int main() {
    Logger l;
    l.templateMethod();
    assert(l.log.size() == 3);
    return 0;
}