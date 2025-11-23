#ifndef Z8_H
#define Z8_H

#include <string>
#include <stdexcept>

class Z8Number {
public:
    // Конструкторы
    Z8Number();                             // ноль: "a"
    explicit Z8Number(const std::string& s); // из строки, например "ba"

    // Арифметические операторы
    Z8Number operator+(const Z8Number& other) const;
    Z8Number operator-(const Z8Number& other) const;
    Z8Number operator*(const Z8Number& other) const;

    // Сравнение
    bool operator==(const Z8Number& other) const;
    bool operator!=(const Z8Number& other) const { return !(*this == other); }

    // Вывод
    std::string toString() const;

private:
    std::string digits; // всегда без ведущих 'a', кроме случая "a"

    // Вспомогательные функции
    static int charToIndex(char c);
    static char indexToChar(int idx);
    static void validateString(const std::string& s);
    static std::string normalize(const std::string& s);
    static std::string addImpl(const std::string& x, const std::string& y);
    static std::string subtractImpl(const std::string& x, const std::string& y);
    static std::string multiplyImpl(const std::string& x, const std::string& y);
    static bool lessThanOrEqual(const std::string& a, const std::string& b);
    static std::string padLeft(const std::string& s, size_t len, char pad = 'a');
};

// Исключение для переполнения
class OverflowError : public std::overflow_error {
public:
    OverflowError() : std::overflow_error("Result exceeds 8 digits") {}
};

#endif // Z8_H