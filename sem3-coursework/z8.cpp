#include "z8.h"
#include <algorithm>
#include <cctype>
#include <stdexcept>

// --- Вспомогательные отображения (изоморфизм с Z_8) ---
int Z8Number::charToIndex(char c) {
    switch (c) {
    case 'a': return 0;
    case 'b': return 1;
    case 'd': return 2;
    case 'h': return 3;
    case 'c': return 4;
    case 'e': return 5;
    case 'g': return 6;
    case 'f': return 7;
    default: throw std::invalid_argument("Invalid digit: " + std::string(1, c));
    }
}

char Z8Number::indexToChar(int idx) {
    const char order[8] = { 'a','b','d','h','c','e','g','f' };
    idx = (idx % 8 + 8) % 8; // нормализация
    return order[idx];
}

// --- Валидация строки ---
void Z8Number::validateString(const std::string& s) {
    if (s.empty())
        throw std::invalid_argument("Empty string is not allowed");
    if (s.size() > 8)
        throw std::invalid_argument("Number cannot exceed 8 digits");
    for (char c : s) {
        if (c != 'a' && c != 'b' && c != 'c' && c != 'd' &&
            c != 'e' && c != 'f' && c != 'g' && c != 'h')
            throw std::invalid_argument("Invalid character: " + std::string(1, c));
    }
}

// --- Нормализация: убрать ведущие 'a', но оставить хотя бы один символ ---
std::string Z8Number::normalize(const std::string& s) {
    size_t start = 0;
    while (start < s.size() - 1 && s[start] == 'a') // оставляем последний, даже если 'a'
        ++start;
    return s.substr(start);
}

// --- Конструкторы ---
Z8Number::Z8Number() : digits("a") {}

Z8Number::Z8Number(const std::string& s) {
    validateString(s);
    digits = normalize(s);
}

// --- Сравнение ---
bool Z8Number::operator==(const Z8Number& other) const {
    return normalize(digits) == normalize(other.digits);
}

// --- Приватные утилиты ---
std::string Z8Number::padLeft(const std::string& s, size_t len, char pad) {
    if (s.length() >= len) return s;
    return std::string(len - s.length(), pad) + s;
}

// --- Сложение (многоразрядное) ---
std::string Z8Number::addImpl(const std::string& x, const std::string& y) {
    size_t len = std::max(x.size(), y.size());
    std::string a = padLeft(x, len, 'a');
    std::string b = padLeft(y, len, 'a');

    std::string result;
    bool carry = false;

    for (int i = len - 1; i >= 0; --i) {
        int sum = charToIndex(a[i]) + charToIndex(b[i]) + (carry ? 1 : 0);
        carry = (sum >= 8);
        result.push_back(indexToChar(sum % 8));
    }

    if (carry) {
        result.push_back('b'); // b = 1
    }

    std::reverse(result.begin(), result.end());
    result = normalize(result);

    if (result.size() > 8)
        throw OverflowError();

    return result;
}

// --- Сравнение для вычитания ---
bool Z8Number::lessThanOrEqual(const std::string& a, const std::string& b) {
    std::string na = normalize(a);
    std::string nb = normalize(b);
    if (na.size() != nb.size())
        return na.size() < nb.size();
    return na <= nb; // лексикографически, т.к. порядок символов согласован с Z8
}

// --- Вычитание (предполагаем, что a >= b) ---
std::string Z8Number::subtractImpl(const std::string& x, const std::string& y) {
    if (!lessThanOrEqual(y, x))
        throw std::domain_error("Negative results are not supported");

    size_t len = x.size();
    std::string a = x;
    std::string b = padLeft(y, len, 'a');

    std::string result;
    bool borrow = false;

    for (int i = len - 1; i >= 0; --i) {
        int ai = charToIndex(a[i]);
        int bi = charToIndex(b[i]);

        if (borrow) {
            if (ai == 0) {
                ai = 8;
            }
            ai--;
            borrow = false;
        }

        if (ai < bi) {
            ai += 8;
            borrow = true;
        }

        int diff = ai - bi;
        result.push_back(indexToChar(diff));
    }

    std::reverse(result.begin(), result.end());
    result = normalize(result);

    if (result.size() > 8)
        throw OverflowError();

    return result;
}

// --- Умножение (школьный алгоритм) ---
std::string Z8Number::multiplyImpl(const std::string& x, const std::string& y) {
    if (x == "a" || y == "a")
        return "a";

    std::string result = "a";

    for (int i = y.size() - 1; i >= 0; --i) {
        char y_digit = y[i];
        int y_val = charToIndex(y_digit);

        // Умножаем x на y_digit
        std::string term = "a";
        for (int k = 0; k < y_val; ++k) {
            term = addImpl(term, x);
        }

        // Сдвигаем влево на (y.size() - 1 - i) позиций (добавляем 'a' справа)
        term += std::string(y.size() - 1 - i, 'a');

        result = addImpl(result, term);

        if (result.size() > 8)
            throw OverflowError();
    }

    return normalize(result);
}

// --- Публичные операторы ---
Z8Number Z8Number::operator+(const Z8Number& other) const {
    return Z8Number(addImpl(digits, other.digits));
}

Z8Number Z8Number::operator-(const Z8Number& other) const {
    return Z8Number(subtractImpl(digits, other.digits));
}

Z8Number Z8Number::operator*(const Z8Number& other) const {
    return Z8Number(multiplyImpl(digits, other.digits));
}

// --- Вывод ---
std::string Z8Number::toString() const {
    return digits;
}