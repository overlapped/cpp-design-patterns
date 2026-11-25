#pragma once
#include <map>
#include <memory>
#include <string>

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
        auto it = cache_.find(c);
        if (it != cache_.end()) return it->second.get();
        auto g = std::unique_ptr<Glyph>(new Glyph(c));
        const Glyph* raw = g.get();
        cache_[c] = std::move(g);
        return raw;
    }
    std::size_t size() const { return cache_.size(); }
private:
    std::map<char, std::unique_ptr<Glyph>> cache_;
};