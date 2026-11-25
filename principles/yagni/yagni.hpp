#pragma once
#include <string>
#include <vector>
#include <numeric>

// ❌ Плохо: класс с кучей «на будущее» методов, которые никто не вызывает
namespace bad {
    class Report {
    public:
        void addLine(const std::string& s) { lines_.push_back(s); }

        // Эти методы «на всякий случай» — YAGNI-нарушение
        void exportToPdf()   { /* TODO */ }
        void exportToHtml()  { /* TODO */ }
        void sendByEmail()   { /* TODO */ }
        void scheduleDaily() { /* TODO */ }

        int lineCount() const { return static_cast<int>(lines_.size()); }
    private:
        std::vector<std::string> lines_;
    };
}

// ✅ Хорошо: только то, что реально нужно сейчас
namespace good {
    class Report {
    public:
        void addLine(const std::string& s) { lines_.push_back(s); }
        int lineCount() const { return static_cast<int>(lines_.size()); }
    private:
        std::vector<std::string> lines_;
    };
}