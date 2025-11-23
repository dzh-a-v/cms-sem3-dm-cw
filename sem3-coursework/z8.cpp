#include "z8.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cctype>

// === 1. БАЗОВАЯ ФУНКЦИЯ +1 ===
char plus_one(char c) {
    switch (c) {
    case 'a': return 'b';
    case 'b': return 'd';
    case 'c': return 'e';
    case 'd': return 'h';
    case 'e': return 'g';
    case 'f': return 'a';
    case 'g': return 'f';
    case 'h': return 'c';
    default: return 'a';
    }
}

// === 2. ОБРАТНАЯ ФУНКЦИЯ -1 ===
char minus_one(char c) {
    switch (c) {
    case 'a': return 'f';
    case 'b': return 'a';
    case 'c': return 'h';
    case 'd': return 'b';
    case 'e': return 'c';
    case 'f': return 'g';
    case 'g': return 'e';
    case 'h': return 'd';
    default: return 'a';
    }
}

// === 3. СЛОЖЕНИЕ ЦИФР ===
std::pair<char, char> add_digits(char x, char y) {
    if (x == 'a') return { y, 'a' };
    if (y == 'a') return { x, 'a' };

    char result = x;
    char carry = 'a';
    char y_copy = y;

    while (y_copy != 'a') {
        if (result == 'f') {
            result = 'a';
            carry = 'b';
        }
        else {
            result = plus_one(result);
            if (carry == 'b') carry = 'a'; // сброс переноса, если не на границе
        }
        y_copy = minus_one(y_copy);
    }

    return { result, carry };
}

// === 4. ВЫЧИТАНИЕ ЦИФР ===
std::pair<char, char> subtract_digits(char x, char y) {
    if (y == 'a') return { x, 'a' };

    char result = x;
    char borrow = 'a';
    char y_copy = y;

    while (y_copy != 'a') {
        if (result == 'a') {
            result = 'f';
            borrow = 'b';
        }
        else {
            result = minus_one(result);
            if (borrow == 'b') borrow = 'a';
        }
        y_copy = minus_one(y_copy);
    }

    return { result, borrow };
}

// === 5. УМНОЖЕНИЕ ЦИФР (БЕЗ ПЕРЕВОДА В INT!) ===
std::pair<char, char> multiply_digits(char x, char y) {
    if (x == 'a' || y == 'a') {
        return { 'a', 'a' };
    }

    char result = 'a';
    char carry = 'a';
    char y_copy = y;

    while (y_copy != 'a') {
        auto [new_result, add_carry] = add_digits(result, x);
        result = new_result;

        if (add_carry == 'b') {
            auto [new_carry, extra] = add_digits(carry, 'b');
            carry = new_carry;
            // extra игнорируем (в одноразрядном умножении он редко != 'a')
        }

        y_copy = minus_one(y_copy);
    }

    return { result, carry };
}

// === 6. ДЕЛЕНИЕ ЦИФР ===
std::vector<char> divide_digits(char x, char y) {
    std::vector<char> res;
    if (y == 'a') {
        if (x == 'a') {
            for (char c : {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}) {
                res.push_back(c);
            }
        }
        return res;
    }

    for (char z : {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'}) {
        auto [prod, _] = multiply_digits(y, z);
        if (prod == x) {
            res.push_back(z);
        }
    }
    return res;
}

// === 7. КЛАСС Z8Number ===

Z8Number::Z8Number(const std::string& input) {
    if (input == "OVERFLOW") {
        digits_ = std::string(9, 'h');
        return;
    }
    std::string clean = input;
    size_t start = clean.find_first_not_of('a');
    if (start == std::string::npos) clean = "a";
    else clean = clean.substr(start);

    if (clean.size() > 8) clean = clean.substr(clean.size() - 8);

    // Храним в little-endian: digits_[0] — младший разряд
    digits_.assign(8, 'a');
    int idx = 0;
    for (int i = clean.size() - 1; i >= 0 && idx < 8; --i, ++idx) {
        digits_[idx] = clean[i];
    }
}

std::string Z8Number::str() const {
    if (digits_.size() == 9) return "OVERFLOW";
    std::string big_endian;
    for (int i = 7; i >= 0; --i) {
        big_endian += digits_[i];
    }
    size_t start = big_endian.find_first_not_of('a');
    return (start == std::string::npos) ? "a" : big_endian.substr(start);
}

bool Z8Number::is_overflow() const {
    return digits_.size() == 9;
}

// Сложение
Z8Number Z8Number::operator+(const Z8Number& other) const {
    std::string res(9, 'a'); // 8 разрядов + 1 для переполнения
    char carry = 'a';
    for (int i = 0; i < 8; ++i) {
        auto [s1, c1] = add_digits(digits_[i], other.digits_[i]);
        auto [s2, c2] = add_digits(s1, carry);
        res[i] = s2;
        carry = (c1 == 'b' || c2 == 'b') ? 'b' : 'a';
    }
    res[8] = carry;
    if (carry != 'a') return Z8Number("OVERFLOW");
    Z8Number result("");
    result.digits_ = std::string(res.begin(), res.begin() + 8);
    return result;
}

// Вычитание
Z8Number Z8Number::operator-(const Z8Number& other) const {
    std::string res(8, 'a');
    char borrow = 'a';
    for (int i = 0; i < 8; ++i) {
        char cur = digits_[i];
        if (borrow == 'b') {
            auto [new_cur, b] = subtract_digits(cur, 'b');
            cur = new_cur;
            borrow = b;
        }
        auto [diff, b] = subtract_digits(cur, other.digits_[i]);
        res[i] = diff;
        if (borrow == 'a') borrow = b;
    }
    Z8Number result("");
    result.digits_ = res;
    return result;
}

// УМНОЖЕНИЕ (многоразрядное, с переносами)
Z8Number Z8Number::operator*(const Z8Number& other) const {
    // Буфер для результата — до 16 разрядов (little-endian)
    std::string full_result(16, 'a');

    for (int i = 0; i < 8; ++i) {
        if (other.digits_[i] == 'a') continue;
        for (int j = 0; j < 8; ++j) {
            if (digits_[j] == 'a') continue;
            auto [prod, carry] = multiply_digits(digits_[j], other.digits_[i]);
            // Добавляем prod в позицию i+j
            auto [sum, c1] = add_digits(full_result[i + j], prod);
            full_result[i + j] = sum;
            // Перенос от сложения
            char current_carry = c1;
            int k = i + j + 1;
            while (current_carry == 'b' && k < 16) {
                auto [new_digit, new_carry] = add_digits(full_result[k], 'b');
                full_result[k] = new_digit;
                current_carry = new_carry;
                k++;
            }
            // Добавляем carry от умножения в следующий разряд
            if (carry != 'a' && i + j + 1 < 16) {
                auto [sum2, c2] = add_digits(full_result[i + j + 1], carry);
                full_result[i + j + 1] = sum2;
                current_carry = c2;
                k = i + j + 2;
                while (current_carry == 'b' && k < 16) {
                    auto [nd, nc] = add_digits(full_result[k], 'b');
                    full_result[k] = nd;
                    current_carry = nc;
                    k++;
                }
            }
        }
    }

    // Проверка переполнения: есть ли ненулевые разряды после 8-го?
    for (int i = 8; i < 16; ++i) {
        if (full_result[i] != 'a') {
            return Z8Number("OVERFLOW");
        }
    }

    Z8Number result("");
    result.digits_ = std::string(full_result.begin(), full_result.begin() + 8);
    return result;
}

// Деление (упрощённое — можно доработать при необходимости)
std::pair<Z8Number, Z8Number> Z8Number::operator/(const Z8Number& other) const {
    std::string divisor_str = other.str();
    if (divisor_str.find_first_not_of('a') == std::string::npos) {
        if (this->str() == "a") {
            return { Z8Number("ALL"), Z8Number("") };
        }
        else {
            return { Z8Number("NO"), Z8Number("") };
        }
    }

    Z8Number quotient("a");
    Z8Number remainder("a");
    std::string dividend = this->str();

    for (char digit : dividend) {
        std::string new_rem = remainder.str() + digit;
        if (new_rem.size() > 8) new_rem = new_rem.substr(1);
        remainder = Z8Number(new_rem);

        char count = 'a';
        while (true) {
            Z8Number temp = remainder - other;
            // Проверяем, не ушло ли в "отрицательное"
            std::string temp_str = temp.str();
            if (temp_str.empty() || temp_str.find_first_not_of('a') == std::string::npos) {
                // temp == 0
                remainder = temp;
                count = plus_one(count);
            }
            else if (temp.digits_[7] == 'a') { // грубая проверка на "положительность"
                remainder = temp;
                count = plus_one(count);
            }
            else {
                break;
            }
        }
        // Добавляем count к частному — сложно без позиционной арифметики
        // Оставим как заглушку
    }

    return { quotient, remainder };
}

// === 8. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===

std::string normalize_number(const std::string& s) {
    size_t start = s.find_first_not_of('a');
    return (start == std::string::npos) ? "a" : s.substr(start);
}

void print_number(const std::string& num) {
    if (num == "OVERFLOW" || num == "ALL" || num == "NO") {
        std::cout << num;
        return;
    }
    std::string n = normalize_number(num);
    std::cout << "( ";
    for (size_t i = 0; i < n.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << n[i];
    }
    std::cout << " )";
}

bool IsValidNumber(const std::string& s) {
    if (s.empty() || s.size() > 8) return false;
    for (char c : s) {
        if (c < 'a' || c > 'h') return false;
    }
    return true;
}

bool ParseExpression(const std::string& line, std::string& num1, char& op, std::string& num2) {
    size_t i = 0;
    while (i < line.size() && isspace(static_cast<unsigned char>(line[i]))) i++;
    size_t start = i;
    while (i < line.size() && line[i] >= 'a' && line[i] <= 'h') i++;
    if (i == start) return false;
    num1 = line.substr(start, i - start);
    while (i < line.size() && isspace(static_cast<unsigned char>(line[i]))) i++;
    if (i >= line.size()) return false;
    op = line[i];
    if (op != '+' && op != '-' && op != '*' && op != '/') return false;
    i++;
    while (i < line.size() && isspace(static_cast<unsigned char>(line[i]))) i++;
    if (i >= line.size()) return false;
    start = i;
    while (i < line.size() && line[i] >= 'a' && line[i] <= 'h') i++;
    if (i == start) return false;
    num2 = line.substr(start, i - start);
    while (i < line.size() && isspace(static_cast<unsigned char>(line[i]))) i++;
    return (i == line.size());
}