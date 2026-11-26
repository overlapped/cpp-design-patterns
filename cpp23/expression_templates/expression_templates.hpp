#pragma once
#include <cstddef>
#include <vector>
#include <cassert>

// Expression Templates: ленивые вычисления над векторами.
// Вместо того чтобы вычислять v1 + v2 + v3 по шагам (создавая
// временные векторы), мы строим дерево выражений, которое
// вычисляется один раз при присваивании.
//
// v1 + v2 + v3
//   без ET: tmp1 = v1 + v2; tmp2 = tmp1 + v3; result = tmp2;
//   с ET:   result[i] = v1[i] + v2[i] + v3[i] для каждого i;
//
// Экономия: нет временных векторов, одно выделение памяти
// (под result), один проход по данным.

// Базовый класс для всех выражений
template<typename Derived>
struct Expr {
    const Derived& self() const { return static_cast<const Derived&>(*this); }
};

// Вектор — лист дерева выражений
struct Vec : Expr<Vec> {
    std::vector<double> data;

    explicit Vec(std::size_t n) : data(n, 0.0) {}
    explicit Vec(std::vector<double> d) : data(std::move(d)) {}

    std::size_t size() const { return data.size(); }
    double operator[](std::size_t i) const { return data[i]; }
    double& operator[](std::size_t i) { return data[i]; }
};

// Бинарное выражение: Op(a, b)
template<typename L, typename R, typename Op>
struct BinaryExpr : Expr<BinaryExpr<L, R, Op>> {
    const L& lhs;
    const R& rhs;
    Op op;

    BinaryExpr(const L& l, const R& r, Op o) : lhs(l), rhs(r), op(o) {}

    std::size_t size() const { return lhs.size(); }
    double operator[](std::size_t i) const {
        return op(lhs[i], rhs[i]);
    }
};

// Операции
struct AddOp { double operator()(double a, double b) const { return a + b; } };
struct SubOp { double operator()(double a, double b) const { return a - b; } };
struct MulOp { double operator()(double a, double b) const { return a * b; } };

// Перегрузки operator+/-/* для любых Expr
template<typename L, typename R>
auto operator+(const Expr<L>& l, const Expr<R>& r) {
    return BinaryExpr<L, R, AddOp>(l.self(), r.self(), AddOp{});
}

template<typename L, typename R>
auto operator-(const Expr<L>& l, const Expr<R>& r) {
    return BinaryExpr<L, R, SubOp>(l.self(), r.self(), SubOp{});
}

template<typename L, typename R>
auto operator*(const Expr<L>& l, const Expr<R>& r) {
    return BinaryExpr<L, R, MulOp>(l.self(), r.self(), MulOp{});
}

// Присваивание выражения вектору: вычисляем один раз
template<typename E>
Vec& operator+=(Vec& v, const Expr<E>& e) {
    const E& expr = e.self();
    assert(expr.size() == v.size());
    for (std::size_t i = 0; i < v.size(); ++i) {
        v[i] += expr[i];
    }
    return v;
}

// Присваивание: Vec result = v1 + v2 + v3
template<typename E>
Vec& assign(Vec& v, const Expr<E>& e) {
    const E& expr = e.self();
    assert(expr.size() == v.size());
    for (std::size_t i = 0; i < v.size(); ++i) {
        v[i] = expr[i];
    }
    return v;
}

// Хелпер для создания вектора из initializer_list
inline Vec makeVec(std::initializer_list<double> init) {
    return Vec(std::vector<double>(init));
}