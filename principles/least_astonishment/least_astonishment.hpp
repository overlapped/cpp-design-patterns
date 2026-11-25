#pragma once
#include <string>
#include <vector>

// ❌ Плохо: имена и поведение обманывают ожидания
namespace bad {
    class Container {
    public:
        // getSize() — звучит как геттер, но удаляет элементы!
        int getSize() {
            int n = static_cast<int>(data_.size());
            data_.clear();
            return n;
        }
        void add(int x) { data_.push_back(x); }
    private:
        std::vector<int> data_;
    };

    // operator+ вычитает — прямое нарушение ожиданий
    struct Vec2 {
        int x, y;
        Vec2 operator+(const Vec2& o) const { return {x - o.x, y - o.y}; }
    };
}

// ✅ Хорошо: имена честные, побочные эффекты видны из имени
namespace good {
    class Container {
    public:
        int size() const { return static_cast<int>(data_.size()); }

        // Явно говорим, что метод меняет состояние
        std::vector<int> takeAll() {
            std::vector<int> out;
            out.swap(data_);
            return out;
        }
        void add(int x) { data_.push_back(x); }
    private:
        std::vector<int> data_;
    };

    struct Vec2 {
        int x, y;
        Vec2 operator+(const Vec2& o) const { return {x + o.x, y + o.y}; }
    };
}