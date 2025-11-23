#ifndef Z8_H
#define Z8_H

#include <string>
#include <stdexcept>

class Z8Number {
public:
    Z8Number();
    explicit Z8Number(const std::string& s);

    Z8Number operator+(const Z8Number& other) const;
    Z8Number operator-(const Z8Number& other) const;
    Z8Number operator*(const Z8Number& other) const;

    bool operator==(const Z8Number& other) const;
    std::string toString() const;

private:
    std::string digits;

    static char inc(char c);
    static char dec(char c);
    static std::string incNumber(const std::string& num);
    static std::string decNumber(const std::string& num);
    static std::string addNumbers(const std::string& x, const std::string& y);
    static std::string subNumbers(const std::string& x, const std::string& y);
    static std::string mulNumbers(const std::string& x, const std::string& y);
    static std::string normalize(const std::string& s);
    static void validate(const std::string& s);
    static bool isEqual(const std::string& a, const std::string& b);
};

class OverflowError : public std::overflow_error {
public:
    OverflowError() : std::overflow_error("Number exceeds 8 digits") {}
};

#endif