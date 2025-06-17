#ifndef APP_EXCEPTIONS_H
#define APP_EXCEPTIONS_H

#include "Str.h"
#include <iostream>

class AppException {
public:
    virtual Str getMessage() const = 0;
    virtual void showMessage() const = 0;
    virtual ~AppException() {}
};

class AuthException : public AppException {
public:
    AuthException(const Str& msg) : message(msg) {}

    Str getMessage() const override {
        return Str("Authentication Error: ").concat(message);
    }

    void showMessage() const override {
        std::cout << getMessage() << std::endl;
    }

private:
    Str message;
};

class SkillValidationException : public AppException {
public:
    SkillValidationException(const Str& msg) : message(msg) {}

    Str getMessage() const override {
        return Str("Skill Validation Error: ").concat(message);
    }

    void showMessage() const override {
        std::cout << getMessage() << std::endl;
    }

private:
    Str message;
};

class NetworkException : public AppException {
public:
    NetworkException(const Str& msg) : message(msg) {}

    Str getMessage() const override {
        return Str("Network Error: ").concat(message);
    }

    void showMessage() const override {
        std::cout << getMessage() << std::endl;
    }

private:
    Str message;
};
class FileException : public AppException {
public:
    FileException(const Str& msg) : message(msg) {}

    Str getMessage() const override {
        return Str("File Error: ").concat(message);
    }

    void showMessage() const override {
        std::cout << getMessage() << std::endl;
    }

private:
    Str message;
};

#endif // APP_EXCEPTIONS_H
