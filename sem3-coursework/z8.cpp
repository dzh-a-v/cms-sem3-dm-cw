#include "z8.h"
#include <algorithm>
#include <iostream>

// === 1. СООТВЕТСТВИЕ СИМВОЛ ↔ ИНДЕКС (только для таблиц!) ===
namespace {
    const char INDEX_TO_SYMBOL[8] = { 'a', 'b', 'd', 'h', 'c', 'e', 'g', 'f' };
    int SYMBOL_TO_INDEX[256] = {};

    struct TableInitializer {
        TableInitializer() {
            for (int i = 0; i < 256; ++i) SYMBOL_TO_INDEX[i] = -1;
            for (int i = 0; i < 8; ++i) {
                SYMBOL_TO_INDEX[static_cast<unsigned char>(INDEX_TO_SYMBOL[i])] = i;
            }
        }
    } initializer;
}

// === 2. ПРЕДВЫЧИСЛЕННЫЕ ТАБЛИЦЫ ===
namespace {
    char ADD_TABLE[8][8];
    char ADD_CARRY[8][8];
    char MUL_TABLE[8][8];
    char MUL_CARRY[8][8];

    struct ArithmeticTables {
        ArithmeticTables() {
            // Сложение
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    int sum = i + j;
                    ADD_TABLE[i][j] = INDEX_TO_SYMBOL[sum % 8];
                    ADD_CARRY[i][j] = INDEX_TO_SYMBOL[sum / 8];
                }
            }
            // Умножение
            for (int i = 0; i < 8; ++i) {
                for (int j = 0; j < 8; ++j) {
                    int prod = i * j;
                    MUL_TABLE[i][j] = INDEX_TO_SYMBOL[prod % 8];
                    MUL_CARRY[i][j] = INDEX_TO_SYMBOL[prod / 8];
                }
            }
        }
    } tables;
}

// === 3. ОПЕРАЦИИ НАД ЦИФРАМИ (через таблицы) ===

std::pair<char, char> add_digits(char x, char y) {
    int i = SYMBOL_TO_INDEX[static_cast<unsigned char>(x)];
    int j = SYMBOL_TO_INDEX[static_cast<unsigned char>(y)];
    if (i == -1 || j == -1) return { 'a', 'a' };
    return { ADD_TABLE[i][j], ADD_CARRY[i][j] };
}

std::pair<char, char> multiply_digits(char x, char y) {
    int i = SYMBOL_TO_INDEX[static_cast<unsigned char>(x)];
    int j = SYMBOL_TO_INDEX[static_cast<unsigned char>(y)];
    if (i == -1 || j == -1) return { 'a', 'a' };
    return { MUL_TABLE[i][j], MUL_CARRY[i][j] };
}

std::vector<char> divide_digits(char x, char y) {
    std::vector<char> res;
    if (y == 'a') {
        if (x == 'a') {
            for (char c : INDEX_TO_SYMBOL) res.push_back(c);
        }
        return res;
    }
    int y_idx = SYMBOL_TO_INDEX[static_cast<unsigned char>(y)];
    int x_idx = SYMBOL_TO_INDEX[static_cast<unsigned char>(x)];
    for (int z = 0; z < 8; ++z) {
        if ((y_idx * z) % 8 == x_idx) {
            res.push_back(INDEX_TO_SYMBOL[z]);
        }
    }
    return res;
}

// === 4. КЛАСС Z8Number ===

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

    // little-endian: младший разряд — первый
    digits_.assign(8, 'a');
    for (size_t i = 0; i < clean.size(); ++i) {
        digits_[i] = clean[clean.size() - 1 - i];
    }
}

std::string Z8Number::str() const {
    if (digits_.size() == 9) return "OVERFLOW";
    std::string big;
    for (int i = 7; i >= 0; --i) big += digits_[i];
    size_t start = big.find_first_not_of('a');
    return (start == std::string::npos) ? "a" : big.substr(start);
}

bool Z8Number::is_overflow() const {
    return digits_.size() == 9;
}

// Сложение
Z8Number Z8Number::operator+(const Z8Number& other) const {
    std::string res(9, 'a');
    char carry = 'a';
    for (int i = 0; i < 8; ++i) {
        auto [s1, c1] = add_digits(digits_[i], other.digits_[i]);
        auto [s2, c2] = add_digits(s1, carry);
        res[i] = s2;
        carry = (c1 == 'b' || c2 == 'b') ? 'b' : 'a';
    }
    res[8] = carry;
    if (carry != 'a') return Z8Number("OVERFLOW");
    Z8Number r("");
    r.digits_ = std::string(res.begin(), res.begin() + 8);
    return r;
}

// Умножение (школьный метод)
Z8Number Z8Number::operator*(const Z8Number& other) const {
    std::string full(16, 'a'); // 16 разрядов

    for (int i = 0; i < 8; ++i) {
        if (other.digits_[i] == 'a') continue;
        for (int j = 0; j < 8; ++j) {
            if (digits_[j] == 'a') continue;
            auto [prod, carry] = multiply_digits(digits_[j], other.digits_[i]);
            // Добавляем prod к позиции i+j
            auto [sum, c1] = add_digits(full[i + j], prod);
            full[i + j] = sum;
            char current_carry = c1;
            int k = i + j + 1;
            while (current_carry != 'a' && k < 16) {
                auto [new_digit, new_carry] = add_digits(full[k], current_carry);
                full[k] = new_digit;
                current_carry = new_carry;
                k++;
            }
            // Добавляем carry от умножения
            if (carry != 'a' && i + j + 1 < 16) {
                auto [sum2, c2] = add_digits(full[i + j + 1], carry);
                full[i + j + 1] = sum2;
                current_carry = c2;
                k = i + j + 2;
                while (current_carry != 'a' && k < 16) {
                    auto [nd, nc] = add_digits(full[k], current_carry);
                    full[k] = nd;
                    current_carry = nc;
                    k++;
                }
            }
        }
    }

    // Проверка переполнения
    for (int i = 8; i < 16; ++i) {
        if (full[i] != 'a') return Z8Number("OVERFLOW");
    }

    Z8Number result("");
    result.digits_ = std::string(full.begin(), full.begin() + 8);
    return result;
}

// Остальные операции (вычитание, деление) можно оставить как заглушки
// или реализовать аналогично

Z8Number Z8Number::operator-(const Z8Number& other) const {
    // Заглушка
    return Z8Number("a");
}

std::pair<Z8Number, Z8Number> Z8Number::operator/(const Z8Number& other) const {
    // Заглушка
    return { Z8Number("a"), Z8Number("a") };
}

// === 5. ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ===

void print_number(const std::string& num) {
    if (num == "OVERFLOW") {
        std::cout << "OVERFLOW";
        return;
    }
    std::string n = num;
    size_t start = n.find_first_not_of('a');
    if (start == std::string::npos) n = "a";
    else n = n.substr(start);

    std::cout << "( ";
    for (size_t i = 0; i < n.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << n[i];
    }
    std::cout << " )";
}

bool IsValidNumber(const std::string& s) {
    if (s.empty() || s.size() > 8) return false;
    for (char c : s) if (c < 'a' || c > 'h') return false;
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