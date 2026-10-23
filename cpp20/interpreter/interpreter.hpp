#pragma once
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

class Context {
public:
    void set(std::string_view name, int value) { vars_[std::string(name)] = value; }
    int get(std::string_view name) const {
        auto it = vars_.find(std::string(name));
        if (it == vars_.end())
            throw std::runtime_error("unknown variable: " + std::string(name));
        return it->second;
    }
private:
    std::unordered_map<std::string, int> vars_;
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
    explicit VariableExpression(std::string_view name) : name_(name) {}
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

// Удобные фабрики
inline std::unique_ptr<Expression> num(int v) {
    return std::make_unique<NumberExpression>(v);
}

inline std::unique_ptr<Expression> var(std::string_view name) {
    return std::make_unique<VariableExpression>(name);
}

enum class Op { Add, Sub, Mul, Div };

inline std::unique_ptr<Expression> binary(
    Op op, std::unique_ptr<Expression> l, std::unique_ptr<Expression> r) {
    switch (op) {
        case Op::Add: return std::make_unique<AddExpression>(std::move(l), std::move(r));
        case Op::Sub: return std::make_unique<SubtractExpression>(std::move(l), std::move(r));
        case Op::Mul: return std::make_unique<MultiplyExpression>(std::move(l), std::move(r));
        case Op::Div: return std::make_unique<DivideExpression>(std::move(l), std::move(r));
    }
    throw std::logic_error("unknown op");
}