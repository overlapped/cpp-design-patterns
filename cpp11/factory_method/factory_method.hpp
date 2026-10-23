#pragma once
#include <memory>
#include <string>

class Document {
public:
    virtual ~Document() = default;
    virtual std::string type() const = 0;
};

class PdfDocument : public Document {
public:
    std::string type() const override { return "PDF"; }
};

class WordDocument : public Document {
public:
    std::string type() const override { return "Word"; }
};

class DocumentCreator {
public:
    virtual ~DocumentCreator() = default;
    std::unique_ptr<Document> create() {
        auto d = createDocument();
        // общая пост-обработка при необходимости
        return d;
    }
protected:
    virtual std::unique_ptr<Document> createDocument() = 0;
};

class PdfCreator : public DocumentCreator {
protected:
    std::unique_ptr<Document> createDocument() override {
        return std::unique_ptr<Document>(new PdfDocument());
    }
};

class WordCreator : public DocumentCreator {
protected:
    std::unique_ptr<Document> createDocument() override {
        return std::unique_ptr<Document>(new WordDocument());
    }
};