#pragma once
#include <memory>
#include <string>

class Image {
public:
    virtual ~Image() = default;
    virtual std::string display() const = 0;
};

class RealImage : public Image {
public:
    explicit RealImage(std::string name) : name_(std::move(name)) {}
    std::string display() const override { return "real:" + name_; }
private:
    std::string name_;
};

class LazyImageProxy : public Image {
public:
    explicit LazyImageProxy(std::string name) : name_(std::move(name)) {}
    std::string display() const override {
        if (!real_) real_ = std::make_unique<RealImage>(name_);
        return real_->display();
    }
    bool loaded() const { return static_cast<bool>(real_); }
private:
    std::string name_;
    mutable std::unique_ptr<Image> real_;
};