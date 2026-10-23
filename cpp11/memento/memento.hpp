#pragma once
#include <string>
#include <vector>

class EditorMemento {
public:
    explicit EditorMemento(std::string s) : state_(std::move(s)) {}
    const std::string& state() const { return state_; }
private:
    std::string state_;
};

class Editor {
public:
    void type(const std::string& s) { text_ += s; }
    const std::string& text() const { return text_; }
    EditorMemento save() const { return EditorMemento(text_); }
    void restore(const EditorMemento& m) { text_ = m.state(); }
private:
    std::string text_;
};

class History {
public:
    void push(const EditorMemento& m) { stack_.push_back(m); }
    bool undo(Editor& e) {
        if (stack_.empty()) return false;
        e.restore(stack_.back());
        stack_.pop_back();
        return true;
    }
private:
    std::vector<EditorMemento> stack_;
};