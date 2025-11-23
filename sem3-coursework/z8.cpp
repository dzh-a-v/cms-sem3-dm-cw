#include "z8.h"
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <iostream>

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

// === 2. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===

int symbol_index(char c) {
    const char all[] = "abcdefgh";
    for (int i = 0; i < 8; ++i) {
        if (all[i] == c) return i;
    }
    return 0;
}

char symbol_from_index(int idx) {
    const char all[] = "abcdefgh";
    return all[(idx & 7)];
}

// === 3. ЯВНАЯ ИНИЦИАЛИЗАЦИЯ ТАБЛИЦ ===

const char SYMBOLS[8] = { 'a','b','c','d','e','f','g','h' };

// Предвычисленные таблицы — заполняются вручную или генерируются один раз
// Мы вычислим их здесь, используя plus_one и steps_from_a

// Вспомогательная функция: сколько шагов от 'a' до c
int steps_from_a(char c) {
    if (c == 'a') return 0;
    char cur = 'a';
    for (int i = 1; i <= 8; ++i) {
        cur = plus_one(cur);
        if (cur == c) return i;
    }
    return 0;
}

// Применить +1 n раз
char apply_plus_one(char start, int n) {
    char res = start;
    for (int i = 0; i < n; ++i) {
        res = plus_one(res);
    }
    return res;
}

// Генерация таблиц
namespace {
    // Локальные функции для инициализации
    char compute_add_result(char x, char y) {
        int steps_y = steps_from_a(y);
        char res = x;
        for (int i = 0; i < steps_y; ++i) {
            if (res == 'f') {
                res = 'a';
            }
            else {
                res = plus_one(res);
            }
        }
        return res;
    }

    char compute_add_carry(char x, char y) {
        int steps_y = steps_from_a(y);
        char res = x;
        bool carry = false;
        for (int i = 0; i < steps_y; ++i) {
            if (res == 'f') {
                res = 'a';
                carry = true;
            }
            else {
                res = plus_one(res);
            }
        }
        return carry ? 'b' : 'a';
    }

    char compute_mul_result(char x, char y) {
        if (x == 'a' || y == 'a') return 'a';
        int steps_y = steps_from_a(y);
        char sum = 'a';
        for (int i = 0; i < steps_y; ++i) {
            auto [s, c] = add_digits(sum, x);
            sum = s;
        }
        return sum;
    }

    char compute_mul_carry(char x, char y) {
        return 'a'; // упрощённо
    }
}

// Теперь явно инициализируем таблицы
const char ADD_TABLE[8][8] = {
    { compute_add_result('a','a'), compute_add_result('a','b'), compute_add_result('a','c'), compute_add_result('a','d'), compute_add_result('a','e'), compute_add_result('a','f'), compute_add_result('a','g'), compute_add_result('a','h') },
    { compute_add_result('b','a'), compute_add_result('b','b'), compute_add_result('b','c'), compute_add_result('b','d'), compute_add_result('b','e'), compute_add_result('b','f'), compute_add_result('b','g'), compute_add_result('b','h') },
    { compute_add_result('c','a'), compute_add_result('c','b'), compute_add_result('c','c'), compute_add_result('c','d'), compute_add_result('c','e'), compute_add_result('c','f'), compute_add_result('c','g'), compute_add_result('c','h') },
    { compute_add_result('d','a'), compute_add_result('d','b'), compute_add_result('d','c'), compute_add_result('d','d'), compute_add_result('d','e'), compute_add_result('d','f'), compute_add_result('d','g'), compute_add_result('d','h') },
    { compute_add_result('e','a'), compute_add_result('e','b'), compute_add_result('e','c'), compute_add_result('e','d'), compute_add_result('e','e'), compute_add_result('e','f'), compute_add_result('e','g'), compute_add_result('e','h') },
    { compute_add_result('f','a'), compute_add_result('f','b'), compute_add_result('f','c'), compute_add_result('f','d'), compute_add_result('f','e'), compute_add_result('f','f'), compute_add_result('f','g'), compute_add_result('f','h') },
    { compute_add_result('g','a'), compute_add_result('g','b'), compute_add_result('g','c'), compute_add_result('g','d'), compute_add_result('g','e'), compute_add_result('g','f'), compute_add_result('g','g'), compute_add_result('g','h') },
    { compute_add_result('h','a'), compute_add_result('h','b'), compute_add_result('h','c'), compute_add_result('h','d'), compute_add_result('h','e'), compute_add_result('h','f'), compute_add_result('h','g'), compute_add_result('h','h') }
};

const char ADD_CARRY[8][8] = {
    { compute_add_carry('a','a'), compute_add_carry('a','b'), compute_add_carry('a','c'), compute_add_carry('a','d'), compute_add_carry('a','e'), compute_add_carry('a','f'), compute_add_carry('a','g'), compute_add_carry('a','h') },
    { compute_add_carry('b','a'), compute_add_carry('b','b'), compute_add_carry('b','c'), compute_add_carry('b','d'), compute_add_carry('b','e'), compute_add_carry('b','f'), compute_add_carry('b','g'), compute_add_carry('b','h') },
    { compute_add_carry('c','a'), compute_add_carry('c','b'), compute_add_carry('c','c'), compute_add_carry('c','d'), compute_add_carry('c','e'), compute_add_carry('c','f'), compute_add_carry('c','g'), compute_add_carry('c','h') },
    { compute_add_carry('d','a'), compute_add_carry('d','b'), compute_add_carry('d','c'), compute_add_carry('d','d'), compute_add_carry('d','e'), compute_add_carry('d','f'), compute_add_carry('d','g'), compute_add_carry('d','h') },
    { compute_add_carry('e','a'), compute_add_carry('e','b'), compute_add_carry('e','c'), compute_add_carry('e','d'), compute_add_carry('e','e'), compute_add_carry('e','f'), compute_add_carry('e','g'), compute_add_carry('e','h') },
    { compute_add_carry('f','a'), compute_add_carry('f','b'), compute_add_carry('f','c'), compute_add_carry('f','d'), compute_add_carry('f','e'), compute_add_carry('f','f'), compute_add_carry('f','g'), compute_add_carry('f','h') },
    { compute_add_carry('g','a'), compute_add_carry('g','b'), compute_add_carry('g','c'), compute_add_carry('g','d'), compute_add_carry('g','e'), compute_add_carry('g','f'), compute_add_carry('g','g'), compute_add_carry('g','h') },
    { compute_add_carry('h','a'), compute_add_carry('h','b'), compute_add_carry('h','c'), compute_add_carry('h','d'), compute_add_carry('h','e'), compute_add_carry('h','f'), compute_add_carry('h','g'), compute_add_carry('h','h') }
};

const char MUL_TABLE[8][8] = {
    { compute_mul_result('a','a'), compute_mul_result('a','b'), compute_mul_result('a','c'), compute_mul_result('a','d'), compute_mul_result('a','e'), compute_mul_result('a','f'), compute_mul_result('a','g'), compute_mul_result('a','h') },
    { compute_mul_result('b','a'), compute_mul_result('b','b'), compute_mul_result('b','c'), compute_mul_result('b','d'), compute_mul_result('b','e'), compute_mul_result('b','f'), compute_mul_result('b','g'), compute_mul_result('b','h') },
    { compute_mul_result('c','a'), compute_mul_result('c','b'), compute_mul_result('c','c'), compute_mul_result('c','d'), compute_mul_result('c','e'), compute_mul_result('c','f'), compute_mul_result('c','g'), compute_mul_result('c','h') },
    { compute_mul_result('d','a'), compute_mul_result('d','b'), compute_mul_result('d','c'), compute_mul_result('d','d'), compute_mul_result('d','e'), compute_mul_result('d','f'), compute_mul_result('d','g'), compute_mul_result('d','h') },
    { compute_mul_result('e','a'), compute_mul_result('e','b'), compute_mul_result('e','c'), compute_mul_result('e','d'), compute_mul_result('e','e'), compute_mul_result('e','f'), compute_mul_result('e','g'), compute_mul_result('e','h') },
    { compute_mul_result('f','a'), compute_mul_result('f','b'), compute_mul_result('f','c'), compute_mul_result('f','d'), compute_mul_result('f','e'), compute_mul_result('f','f'), compute_mul_result('f','g'), compute_mul_result('f','h') },
    { compute_mul_result('g','a'), compute_mul_result('g','b'), compute_mul_result('g','c'), compute_mul_result('g','d'), compute_mul_result('g','e'), compute_mul_result('g','f'), compute_mul_result('g','g'), compute_mul_result('g','h') },
    { compute_mul_result('h','a'), compute_mul_result('h','b'), compute_mul_result('h','c'), compute_mul_result('h','d'), compute_mul_result('h','e'), compute_mul_result('h','f'), compute_mul_result('h','g'), compute_mul_result('h','h') }
};

const char MUL_CARRY[8][8] = {
    { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' },
    { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' },
    { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' },
    { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' },
    { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' },
    { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' },
    { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' },
    { 'a', 'a', 'a', 'a', 'a', 'a', 'a', 'a' }
};

// === 4. ОПЕРАЦИИ НАД ЦИФРАМИ ===

std::pair<char, char> add_digits(char x, char y) {
    int i = symbol_index(x);
    int j = symbol_index(y);
    return { ADD_TABLE[i][j], ADD_CARRY[i][j] };
}

std::pair<char, char> subtract_digits(char x, char y) {
    int y_idx = symbol_index(y);
    int x_idx = symbol_index(x);
    for (int z = 0; z < 8; ++z) {
        if (ADD_TABLE[z][y_idx] == x && ADD_CARRY[z][y_idx] == 'a') {
            return { SYMBOLS[z], 'a' };
        }
    }
    for (int z = 0; z < 8; ++z) {
        if (ADD_TABLE[z][y_idx] == x && ADD_CARRY[z][y_idx] == 'b') {
            return { SYMBOLS[z], 'b' };
        }
    }
    return { 'a', 'a' };
}

std::pair<char, char> multiply_digits(char x, char y) {
    int i = symbol_index(x);
    int j = symbol_index(y);
    return { MUL_TABLE[i][j], MUL_CARRY[i][j] };
}

std::vector<char> divide_digits(char x, char y) {
    std::vector<char> res;
    if (y == 'a') {
        if (x == 'a') {
            for (int i = 0; i < 8; ++i) res.push_back(SYMBOLS[i]);
        }
        return res;
    }
    int y_idx = symbol_index(y);
    int x_idx = symbol_index(x);
    for (int z = 0; z < 8; ++z) {
        if (MUL_TABLE[y_idx][z] == x) {
            res.push_back(SYMBOLS[z]);
        }
    }
    return res;
}

// === 5. МНОГОРАЗРЯДНЫЕ ОПЕРАЦИИ ===

Z8Number::Z8Number(const std::string& input) {
    if (input.size() > 8) {
        digits_ = input.substr(input.size() - 8);
    }
    else {
        digits_ = std::string(8 - input.size(), 'a') + input;
    }
}

std::string Z8Number::str() const {
    return normalize_number(digits_);
}

std::string normalize_number(const std::string& s) {
    size_t start = 0;
    while (start < s.length() - 1 && s[start] == 'a') {
        start++;
    }
    return s.substr(start);
}

bool is_greater_or_equal(const std::string& a_raw, const std::string& b_raw) {
    std::string a = a_raw, b = b_raw;
    if (a.size() > 8) a = a.substr(a.size() - 8);
    if (b.size() > 8) b = b.substr(b.size() - 8);
    a = std::string(8 - a.size(), 'a') + a;
    b = std::string(8 - b.size(), 'a') + b;

    for (int i = 0; i < 8; ++i) {
        if (a[i] != b[i]) {
            auto [_, borrow] = subtract_digits(a[i], b[i]);
            return borrow == 'a';
        }
    }
    return true;
}

Z8Number Z8Number::operator+(const Z8Number& other) const {
    std::string res(8, 'a');
    char carry = 'a';
    for (int i = 7; i >= 0; --i) {
        auto [sum1, c1] = add_digits(digits_[i], other.digits_[i]);
        auto [sum2, c2] = add_digits(sum1, carry);
        res[i] = sum2;
        carry = (c1 == 'b' || c2 == 'b') ? 'b' : 'a';
    }
    return Z8Number(res);
}

Z8Number Z8Number::operator-(const Z8Number& other) const {
    std::string res(8, 'a');
    char borrow = 'a';
    for (int i = 7; i >= 0; --i) {
        char cur = digits_[i];
        if (borrow == 'b') {
            auto [new_cur, new_borrow] = subtract_digits(cur, 'b');
            cur = new_cur;
            borrow = new_borrow;
        }
        auto [diff, b] = subtract_digits(cur, other.digits_[i]);
        res[i] = diff;
        if (borrow == 'a') borrow = b;
    }
    return Z8Number(res);
}

Z8Number Z8Number::operator*(const Z8Number& other) const {
    Z8Number result(std::string(8, 'a'));
    for (int i = 0; i < 8; ++i) {
        if (other.digits_[i] == 'a') continue;
        Z8Number partial(std::string(8, 'a'));
        char carry = 'a';
        for (int j = 0; j < 8 - i; ++j) {
            if (i + j >= 8) break;
            auto [prod, pc] = multiply_digits(digits_[j], other.digits_[i]);
            auto [sum, sc] = add_digits(prod, carry);
            partial.digits_[i + j] = sum;
            carry = pc;
        }
        result = result + partial;
    }
    return result;
}

std::pair<Z8Number, Z8Number> Z8Number::operator/(const Z8Number& other) const {
    std::string divisor = other.str();
    if (divisor.find_first_not_of('a') == std::string::npos) {
        std::string dividend = this->str();
        if (dividend.find_first_not_of('a') == std::string::npos) {
            return { Z8Number("all"), Z8Number("") };
        }
        else {
            return { Z8Number("no"), Z8Number("") };
        }
    }

    Z8Number quotient(std::string(8, 'a'));
    Z8Number remainder(std::string(8, 'a'));
    std::string dividend = this->digits_;

    for (int i = 0; i < 8; ++i) {
        std::string rem_str = remainder.str();
        rem_str += dividend[i];
        if (rem_str.length() > 8) rem_str = rem_str.substr(1);
        remainder = Z8Number(rem_str);

        char count = 'a';
        while (is_greater_or_equal(remainder.str(), divisor)) {
            remainder = remainder - other;
            count = plus_one(count);
        }
        quotient.digits_[i] = count;
    }

    return { quotient, remainder };
}

void print_number(const std::string& num) {
    std::string n = normalize_number(num.empty() ? "a" : num);
    std::cout << "( ";
    for (size_t i = 0; i < n.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << n[i];
    }
    std::cout << " )";
}

// === 6. ФУНКЦИИ ДЛЯ MAIN.CPP ===

bool IsValidNumber(const std::string& s) {
    if (s.empty() || s.size() > 8) return false;
    for (char c : s) {
        if (c < 'a' || c > 'h') return false;
    }
    return true;
}

bool ParseExpression(const std::string& line, std::string& num1, char& op, std::string& num2) {
    if (line.empty()) return false;

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