#ifndef Z8_H
#define Z8_H

#include <string>
#include <vector>
#include <utility>

// Базовые функции
char plus_one(char c);

// Работа с цифрами
std::pair<char, char> add_digits(char x, char y);
std::pair<char, char> subtract_digits(char x, char y);
std::pair<char, char> multiply_digits(char x, char y);
std::vector<char> divide_digits(char x, char y);

// Вспомогательные
int steps_from_a(char c);
bool is_greater_or_equal(const std::string& a, const std::string& b);
std::string normalize_number(const std::string& s);

// Класс числа
class Z8Number {
    std::string digits_; // little-endian: [0] — младший разряд

public:
    explicit Z8Number(const std::string& input);
    std::string str() const;
    bool is_overflow() const;

    Z8Number operator+(const Z8Number& other) const;
    Z8Number operator-(const Z8Number& other) const;
    Z8Number operator*(const Z8Number& other) const;
    std::pair<Z8Number, Z8Number> operator/(const Z8Number& other) const;
};

// Ввод/вывод
void print_number(const std::string& num);
bool IsValidNumber(const std::string& s);
bool ParseExpression(const std::string& line, std::string& num1, char& op, std::string& num2);

#endif // Z8_H