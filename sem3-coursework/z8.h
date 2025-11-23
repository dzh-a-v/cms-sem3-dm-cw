#ifndef Z8_H
#define Z8_H

#include <string>
#include <vector>
#include <utility>

// === Символы и таблицы ===
extern const char SYMBOLS[8];           // 'a','b','c','d','e','f','g','h'
extern const char ADD_TABLE[8][8];
extern const char ADD_CARRY[8][8];
extern const char MUL_TABLE[8][8];
extern const char MUL_CARRY[8][8];

// === Базовые операции ===
char plus_one(char c);
int symbol_index(char c);                 // только для lookup в таблицах!
char symbol_from_index(int idx);

std::pair<char, char> add_digits(char x, char y);
std::pair<char, char> subtract_digits(char x, char y);
std::pair<char, char> multiply_digits(char x, char y);
std::vector<char> divide_digits(char x, char y);

bool is_greater_or_equal(const std::string& a, const std::string& b);
std::string normalize_number(const std::string& s);

class Z8Number {
    std::string digits_;

public:
    explicit Z8Number(const std::string& input);
    std::string str() const;

    Z8Number operator+(const Z8Number& other) const;
    Z8Number operator-(const Z8Number& other) const;
    Z8Number operator*(const Z8Number& other) const;
    std::pair<Z8Number, Z8Number> operator/(const Z8Number& other) const;
};

void print_number(const std::string& num);

// === Функции для main.cpp ===
bool IsValidNumber(const std::string& s);
bool ParseExpression(const std::string& line, std::string& num1, char& op, std::string& num2);

#endif // Z8_H