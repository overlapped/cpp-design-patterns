#include "command.hpp"
#include <cassert>

int main() {
    Light light;
    RemoteControl remote;
    remote.submit(std::unique_ptr<Command>(new LightOnCommand(light)));
    assert(light.state == "on");
    remote.undoLast();
    assert(light.state == "off");
    return 0;
}