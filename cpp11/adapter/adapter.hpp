#pragma once
#include <string>

// Целевой интерфейс
class Printer {
public:
    virtual ~Printer() = default;
    virtual std::string print() const = 0;
};

// Сторонний класс с несовместимым интерфейсом
class LegacyPrinter {
public:
    std::string printLegacy() const { return "legacy"; }
};

// Adapter через композицию
class LegacyPrinterAdapter : public Printer {
public:
    explicit LegacyPrinterAdapter(const LegacyPrinter& l) : legacy_(l) {}
    std::string print() const override { return legacy_.printLegacy(); }
private:
    LegacyPrinter legacy_;
};