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
    std::unique_ptr<Document> create() { return createDocument(); }
protected:
    virtual std::unique_ptr<Document> createDocument() = 0;
};

class PdfCreator : public DocumentCreator {
protected:
    std::unique_ptr<Document> createDocument() override {
        return std::make_unique<PdfDocument>();
    }
};

class WordCreator : public DocumentCreator {
protected:
    std::unique_ptr<Document> createDocument() override {
        return std::make_unique<WordDocument>();
    }
};

enum class DocType { Pdf, Word };

inline std::unique_ptr<DocumentCreator> makeCreator(DocType t) {
    switch (t) {
        case DocType::Pdf:  return std::make_unique<PdfCreator>();
        case DocType::Word: return std::make_unique<WordCreator>();
    }
    return nullptr;
}