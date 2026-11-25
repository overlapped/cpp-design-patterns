#pragma once
#include <string>
#include <fstream>
#include <sstream>

// ❌ Плохо: класс делает и бизнес-логику, и сериализацию, и I/O
namespace bad {
    class User {
    public:
        User(std::string name, int age) : name_(std::move(name)), age_(age) {}
        std::string toJson() const {
            std::ostringstream os;
            os << "{\"name\":\"" << name_ << "\",\"age\":" << age_ << "}";
            return os.str();
        }
        void saveToFile(const std::string& path) const {
            std::ofstream f(path);
            f << toJson();
        }
        std::string name() const { return name_; }
        int age() const { return age_; }
    private:
        std::string name_;
        int age_;
    };
}

// ✅ Хорошо: данные, сериализация и персистентность — разделены
namespace good {
    struct User {
        std::string name;
        int age;
    };

    inline std::string toJson(const User& u) {
        std::ostringstream os;
        os << "{\"name\":\"" << u.name << "\",\"age\":" << u.age << "}";
        return os.str();
    }

    class UserRepository {
    public:
        explicit UserRepository(std::string path) : path_(std::move(path)) {}
        void save(const User& u) const {
            std::ofstream f(path_);
            f << toJson(u);
        }
    private:
        std::string path_;
    };
}