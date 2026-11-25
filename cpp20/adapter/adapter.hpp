#pragma once
#include <string>
#include <string_view>

class Printer {
public:
    virtual ~Printer() = default;
    virtual std::string print() const = 0;
};

class LegacyPrinter {
public:
    std::string printLegacy() const { return "legacy"; }
};

class LegacyPrinterAdapter : public Printer {
public:
    explicit LegacyPrinterAdapter(const LegacyPrinter& l) : legacy_(l) {}
    std::string print() const override { return legacy_.printLegacy(); }
private:
    LegacyPrinter legacy_;
};