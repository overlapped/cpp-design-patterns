#include "hollywood_principle.hpp"
#include <cassert>
#include <iostream>
#include <vector>

class MyPipeline : public good::Pipeline {
public:
    std::vector<int> order;

protected:
    void step1() override { order.push_back(1); }
    void step2() override { order.push_back(2); }
    void step3() override { order.push_back(3); }
};

int main() {
    // ❌ Плохо: клиент сам вызывает fire()
    bad::EventSource src;
    int badCalls = 0;
    src.addHandler([&](const std::string&) { ++badCalls; });
    src.fire("event");
    assert(badCalls == 1);

    // ✅ Хорошо: фреймворк сам вызывает обработчик через run()
    good::EventLoop loop;
    int goodCalls = 0;
    loop.onEvent([&](const std::string&) { ++goodCalls; });
    loop.run({"a", "b", "c"});   // фреймворк «звонит» клиенту
    assert(goodCalls == 3);

    // ✅ Pipeline: фреймворк управляет порядком шагов
    MyPipeline p;
    p.execute();                 // фреймворк вызывает step1/2/3
    assert((p.order == std::vector<int>{1, 2, 3}));

    std::cout << "Hollywood Principle: framework calls you\n";
    return 0;
}