#include "z8.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

bool Z8Number::getNeg() const {
    return isNegative;
}

// --- базовые операции на символах ---
char Z8Number::inc(char c) {
    switch (c) {
    case 'a': return 'b';
    case 'b': return 'd';
    case 'c': return 'e';
    case 'd': return 'h';
    case 'e': return 'g';
    case 'f': return 'a';
    case 'g': return 'f';
    case 'h': return 'c';
    default: throw std::invalid_argument("Invalid digit");
    }
}

char Z8Number::dec(char c) {
    switch (c) {
    case 'a': return 'f';
    case 'b': return 'a';
    case 'c': return 'h';
    case 'd': return 'b';
    case 'e': return 'c';
    case 'f': return 'g';
    case 'g': return 'e';
    case 'h': return 'd';
    default: throw std::invalid_argument("Invalid digit");
    }
}

// --- нормализация: убрать лишние 'a' в начале ---
std::string Z8Number::normalize(const std::string& s) {
    size_t i = s[0] == '-' ? 1 : 0;
    while (i < s.length() - 1 && s[i] == 'a')
        ++i;
    return s.substr(i);
}

// --- сравнение строк ---
bool Z8Number::isEqual(const std::string& a, const std::string& b) {
    return normalize(a) == normalize(b);
}

// --- валидация ---
void Z8Number::validate(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("Empty number");
    if (s.size() > 8 && s[0] != '-' || s.size() > 9) throw std::invalid_argument("More than 8 digits");
    for (char c : s) {
        if (c != 'a' && c != 'b' && c != 'c' && c != 'd' &&
            c != 'e' && c != 'f' && c != 'g' && c != 'h' && c != '-')
            throw std::invalid_argument("Invalid character");
    }
}

// --- конструкторы ---
Z8Number::Z8Number() : digits("a"), isNegative(false) {}
Z8Number::Z8Number(const std::string& s) {
    validate(s);
    digits = normalize(s);
    isNegative = s[0] == '-' ? true : false;
}
Z8Number::Z8Number(const Z8Number& other) {
    digits = other.digits;
    isNegative = other.isNegative;
}

// --- операторы сравнения ---
bool Z8Number::operator==(const Z8Number& other) const {
    return isEqual(digits, other.digits);
}

// --- инкремент многоразрядного числа ---
std::string Z8Number::incNumber(const std::string& num) {
    std::string res = num;
    int i = static_cast<int>(res.size()) - 1;
    while (i >= 0) {
        char old = res[i];
        res[i] = inc(old);
        if (old != 'f') { // f -> a, значит, был перенос
            break;
        }
        if (i == 0) {
            res = 'b' + res; // добавляем новый разряд
            if (res.size() > 8)
                throw OverflowError();
            break;
        }
        --i;
    }
    return normalize(res);
}

// --- декремент многоразрядного числа ---
std::string Z8Number::decNumber(const std::string& num) {
    if (isEqual(num, "a"))
        throw std::domain_error("Cannot decrement zero");
    std::string res = num;
    int i = static_cast<int>(res.size()) - 1;
    while (i >= 0) {
        char old = res[i];
        res[i] = dec(old);
        if (old != 'a') { // a -> f, значит, нужно занять
            break;
        }
        --i;
    }
    return normalize(res);
}

// --- Сравнение: x >= y ? ---
bool Z8Number::greaterOrEqual(const std::string& x, const std::string& y) {
    if (isEqual(x, y)) return true;
    if (isEqual(y, "a")) return true;  // всё >= 0
    if (isEqual(x, "a")) return false; // 0 < y (y != 0)
    try {
        subNumbers(x, y); // если не исключение — x >= y
        return true;
    }
    catch (...) {
        return false;
    }
}

// --- сложение: x + y = x, затем y раз inc ---
std::string Z8Number::addNumbers(const std::string& x, const std::string& y) {
    if (isEqual(y, "a")) return x;
    std::string res = greaterOrEqual(x, y) ? x : y;
    std::string counter = "a";
    std::string countTimes = greaterOrEqual(x, y) ? y : x;
    while (!isEqual(counter, countTimes)) {
        res = incNumber(res);
        counter = incNumber(counter);
    }
    return res;
}

// --- вычитание: x - y = x, затем y раз dec ---
std::string Z8Number::subNumbers(const std::string& x, const std::string& y) {
    if (isEqual(y, "a")) return x;
    if (isEqual(x, y)) return "a";
    std::string res = x;
    std::string counter = "a";
    while (!isEqual(counter, y)) {
        res = decNumber(res);
        counter = incNumber(counter);
    }
    return res;
}

// --- умножение: x * y = y раз сложить x ---
std::string Z8Number::mulNumbers(const std::string& x, const std::string& y) {
    if (isEqual(x, "a") || isEqual(y, "a")) return "a";
    std::string res = greaterOrEqual(x, y) ? x : y;
    std::string counter = "b";
    std::string countTimes = greaterOrEqual(x, y) ? y : x;
    while (!isEqual(counter, countTimes)) {
        res = addNumbers(res, x);
        counter = incNumber(counter);
    }
    return res;
}

// --- Деление с остатком ---
std::string Z8Number::divNumbers(const std::string& x, const std::string& y) {
    if (isEqual(y, "a")) {
        throw std::domain_error("Division by zero");
    }
    if (isEqual(x, "a")) {
        return "a";
    }

    std::string quotient = "a";
    std::string remainder = x;

    while (greaterOrEqual(remainder, y)) {
        remainder = subNumbers(remainder, y);
        quotient = incNumber(quotient);
    }

    if (isEqual(remainder, "a")) {
        return quotient;
    }
    else {
        return quotient + "." + remainder;
    }
}

std::string Z8Number::divide(const Z8Number& divisor) const {
    return divNumbers(digits, divisor.digits);
}

Z8Number Z8Number::operator+(const Z8Number& o) const {
    return Z8Number(addNumbers(digits, o.digits));
}

Z8Number Z8Number::operator-(const Z8Number& o) const {
    return Z8Number(subNumbers(digits, o.digits));
}

Z8Number Z8Number::operator*(const Z8Number& o) const {
    return Z8Number(mulNumbers(digits, o.digits));
}

Z8Number& Z8Number::operator=(const Z8Number& other) {
    digits = other.digits;
    isNegative = other.isNegative;
    return *this;
}

std::string Z8Number::toString() const {
    return digits;
}

void calculate(const Z8Number& a, const Z8Number& b, std::string op) {
    if (!(a.isNegative) && (!(b.isNegative))) {
        try {
            if (op == "+") {
                std::cout << "Result: " << (a + b).toString() << "\n";
            }
            else if (op == "-") {
                std::cout << "Result: " << (a - b).toString() << "\n";
            }
            else if (op == "*") {
                std::cout << "Result: " << (a * b).toString() << "\n";
            }
            else if (op == "/") {
                std::cout << "Result: " << a.divide(b) << "\n";
            }
            else {
                std::cerr << "Unknown operator\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    else if ((a.isNegative) && (!(b.isNegative)) || (!(a.isNegative)) && (b.isNegative)) {
        Z8Number A, B;
        if ((a.isNegative) && (!(b.isNegative))) {
            A = a;
            B = b;
        }
        else {
            A = b;
            B = a;
        }
        try {
            if ((a.isNegative) && (!(b.isNegative))) {
                if (op == "+") {
                    std::cout << "Result: " << (A - B).toString() << "\n";
                }
                else if (op == "-") {
                    std::cout << "Result: " << (A + B).toString() << "\n";
                }
                else if (op == "*") {
                    std::cout << "Result: -" << (A * B).toString() << "\n";
                }
                else if (op == "/") {
                    std::cout << "Result: -" << A.divide(B) << "\n";
                }
                else {
                    std::cerr << "Unknown operator\n";
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    else {
        try {
            if (op == "+") {
                std::cout << "Result: -" << (a + b).toString() << "\n";
            }
            else if (op == "-") {
                try {
                    std::cout << "Result: " << (a - b).toString() << "\n";
                }
                catch (...) {
                    std::cout << "Result: " << (b - a).toString() << "\n";
                }
            }
            else if (op == "*") {
                std::cout << "Result: " << (a * b).toString() << "\n";
            }
            else if (op == "/") {
                std::cout << "Result: " << a.divide(b) << "\n";
            }
            else {
                std::cerr << "Unknown operator\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}