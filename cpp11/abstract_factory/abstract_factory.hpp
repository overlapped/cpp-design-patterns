#pragma once
#include <memory>

class Button {
public:
    virtual ~Button() = default;
    virtual void click() = 0;
};

class Checkbox {
public:
    virtual ~Checkbox() = default;
    virtual void check() = 0;
};

class WinButton : public Button { public: void click() override {} };
class MacButton : public Button { public: void click() override {} };
class WinCheckbox : public Checkbox { public: void check() override {} };
class MacCheckbox : public Checkbox { public: void check() override {} };

class GUIFactory {
public:
    virtual ~GUIFactory() = default;
    virtual std::unique_ptr<Button> createButton() = 0;
    virtual std::unique_ptr<Checkbox> createCheckbox() = 0;
};

class WinFactory : public GUIFactory {
public:
    std::unique_ptr<Button> createButton() override {
        return std::unique_ptr<Button>(new WinButton());
    }
    std::unique_ptr<Checkbox> createCheckbox() override {
        return std::unique_ptr<Checkbox>(new WinCheckbox());
    }
};

class MacFactory : public GUIFactory {
public:
    std::unique_ptr<Button> createButton() override {
        return std::unique_ptr<Button>(new MacButton());
    }
    std::unique_ptr<Checkbox> createCheckbox() override {
        return std::unique_ptr<Checkbox>(new MacCheckbox());
    }
};