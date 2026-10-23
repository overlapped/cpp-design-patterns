#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class Glyph {
public:
    explicit Glyph(char c) : ch(c) {}
    char value() const { return ch; }
private:
    char ch;
};

class GlyphFactory {
public:
    const Glyph* get(char c) {
        if (cache_.contains(c)) return cache_.at(c).get();
        auto g = std::make_unique<Glyph>(c);
        const Glyph* raw = g.get();
        cache_.emplace(c, std::move(g));
        return raw;
    }
    std::size_t size() const { return cache_.size(); }
private:
    std::unordered_map<char, std::unique_ptr<Glyph>> cache_;
};