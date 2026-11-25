#pragma once
#include <memory>
#include <vector>
#include <string>

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class Light {
public:
    std::string state = "off";
    void on() { state = "on"; }
    void off() { state = "off"; }
};

class LightOnCommand : public Command {
public:
    explicit LightOnCommand(Light& l) : light_(l) {}
    void execute() override { light_.on(); }
    void undo() override { light_.off(); }
private:
    Light& light_;
};

class LightOffCommand : public Command {
public:
    explicit LightOffCommand(Light& l) : light_(l) {}
    void execute() override { light_.off(); }
    void undo() override { light_.on(); }
private:
    Light& light_;
};

class RemoteControl {
public:
    void submit(std::unique_ptr<Command> c) {
        c->execute();
        history_.push_back(std::move(c));
    }
    bool undoLast() {
        if (history_.empty()) return false;
        history_.back()->undo();
        history_.pop_back();
        return true;
    }
private:
    std::vector<std::unique_ptr<Command>> history_;
};