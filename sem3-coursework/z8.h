#pragma once

#include <string>
#include <stdexcept>

class Z8Number {
public:
    Z8Number();
    explicit Z8Number(const std::string& s);
    Z8Number(const Z8Number& other);

    Z8Number operator+(const Z8Number& other) const;
    Z8Number operator-(const Z8Number& other) const;
    Z8Number operator*(const Z8Number& other) const;
    std::string divide(const Z8Number& divisor, bool firstIsNeg = false) const;
    Z8Number& operator=(const Z8Number& other);

    bool operator==(const Z8Number& other) const;
    std::string toString() const;

private:
    std::string digits;
    bool isNegative;

    bool getNeg() const;
    static char inc(char c);
    static char dec(char c);
    static std::string incNumber(const std::string& num);
    static std::string decNumber(const std::string& num);
    static std::string addNumbers(const std::string& x, const std::string& y);
    static std::string subNumbers(const std::string& x, const std::string& y);
    static std::string mulNumbers(const std::string& x, const std::string& y);
    static std::string divNumbers(const std::string& x, const std::string& y, bool firstIsNeg = false);
    static bool greaterOrEqual(const std::string& x, const std::string& y);
    friend bool greaterOrEqual(const Z8Number& x, const Z8Number& y);
    friend bool greater(const Z8Number& x, const Z8Number& y);
    static std::string normalize(const std::string& s);
    static void validate(const std::string& s);
    static bool isEqual(const std::string& a, const std::string& b);
    friend void calculate(const Z8Number& a, const Z8Number& b, std::string op);
};

//class OverflowError : public std::overflow_error {
//public:
//    OverflowError() : std::overflow_error("Number exceeds 8 digits") {}
//};