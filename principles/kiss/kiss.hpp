#pragma once
#include <string>
#include <vector>
#include <numeric>

// ❌ Плохо: избыточная абстракция для простой задачи
namespace bad {
    class INumberProcessor {
    public:
        virtual ~INumberProcessor() = default;
        virtual int process(const std::vector<int>&) const = 0;
    };

    class SumProcessor : public INumberProcessor {
    public:
        int process(const std::vector<int>& v) const override {
            int s = 0;
            for (int x : v) s += x;
            return s;
        }
    };

    class ProcessorFactory {
    public:
        static INumberProcessor* create() { return new SumProcessor(); }
    };
}

// ✅ Хорошо: простая функция
namespace good {
    inline int sum(const std::vector<int>& v) {
        return std::accumulate(v.begin(), v.end(), 0);
    }
}