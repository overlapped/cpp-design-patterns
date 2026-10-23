#pragma once
#include <memory>
#include <string>
#include <map>
#include <stdexcept>

// Контекст: сюда можно складывать переменные, если язык их поддерживает.
// Для арифметики он не обязателен, но оставлен как точка расширения.
class Context {
public:
    void set(const std::string& name, int value) { vars_[name] = value; }
    int get(const std::string& name) const {
        auto it = vars_.find(name);
        if (it == vars_.end()) throw std::runtime_error("unknown variable: " + name);
        return it->second;
    }
private:
    std::map<std::string, int> vars_;
};

class Expression {
public:
    virtual ~Expression() = default;
    virtual int interpret(const Context& ctx) const = 0;
};

class NumberExpression : public Expression {
public:
    explicit NumberExpression(int v) : value_(v) {}
    int interpret(const Context&) const override { return value_; }
private:
    int value_;
};

class VariableExpression : public Expression {
public:
    explicit VariableExpression(std::string name) : name_(std::move(name)) {}
    int interpret(const Context& ctx) const override { return ctx.get(name_); }
private:
    std::string name_;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r)
        : left_(std::move(l)), right_(std::move(r)) {}
protected:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;
};

class AddExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;
    int interpret(const Context& ctx) const override {
        return left_->interpret(ctx) + right_->interpret(ctx);
    }
};

class SubtractExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;
    int interpret(const Context& ctx) const override {
        return left_->interpret(ctx) - right_->interpret(ctx);
    }
};

class MultiplyExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;
    int interpret(const Context& ctx) const override {
        return left_->interpret(ctx) * right_->interpret(ctx);
    }
};

class DivideExpression : public BinaryExpression {
public:
    using BinaryExpression::BinaryExpression;
    int interpret(const Context& ctx) const override {
        int r = right_->interpret(ctx);
        if (r == 0) throw std::runtime_error("division by zero");
        return left_->interpret(ctx) / r;
    }
};

// Утилита для краткости в тестах и примерах
inline std::unique_ptr<Expression> num(int v) {
    return std::unique_ptr<Expression>(new NumberExpression(v));
}