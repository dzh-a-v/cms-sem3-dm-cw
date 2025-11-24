#include "z8.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

bool Z8Number::getNeg() const {
    return isNegative;
}

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

// --- normalizing: getting rid of extra 'a' in the beginning ---
std::string Z8Number::normalize(const std::string& s) {
    size_t i = s[0] == '-' ? 1 : 0;
    while (i < s.length() - 1 && s[i] == 'a')
        ++i;
    return s.substr(i);
}

bool Z8Number::isEqual(const std::string& a, const std::string& b) {
    return normalize(a) == normalize(b);
}

void Z8Number::validate(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("Empty number");
    if (s.size() > 8 && s[0] != '-' || s.size() > 9) throw std::invalid_argument("More than 8 digits");
    for (char c : s) {
        if (c != 'a' && c != 'b' && c != 'c' && c != 'd' &&
            c != 'e' && c != 'f' && c != 'g' && c != 'h' && c != '-')
            throw std::invalid_argument("Invalid character");
    }
}

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

bool Z8Number::operator==(const Z8Number& other) const {
    return isEqual(digits, other.digits);
}

std::string Z8Number::incNumber(const std::string& num) {
    std::string res = num;
    int i = static_cast<int>(res.size()) - 1;
    while (i >= 0) {
        char old = res[i];
        res[i] = inc(old);
        if (old != 'f') { // no carry
            break;
        }
        if (i == 0) { // carrying until no need to
            res = 'b' + res;
            if (res.size() > 8)
                throw std::overflow_error("Number exceeds 8 digits");
            break;
        }
        --i;
    }
    if (normalize(res).size() > 8)
        throw std::overflow_error("Number exceeds 8 digits");
    return normalize(res);
}

std::string Z8Number::decNumber(const std::string& num) {
    if (isEqual(num, "a"))
        throw std::domain_error("Cannot decrement zero");
    std::string res = num;
    int i = static_cast<int>(res.size()) - 1;
    while (i >= 0) {
        char old = res[i];
        res[i] = dec(old);
        if (old != 'a') { // a -> f, so we need to take one from higher
            break;
        }
        --i;
    }
    return normalize(res);
}

bool Z8Number::greaterOrEqual(const std::string& x, const std::string& y) {
    if (isEqual(x, y)) return true;
    if (isEqual(y, "a")) return true;
    if (isEqual(x, "a")) return false;
    try {
        subNumbers(x, y);
        return true;
    }
    catch (...) {
        return false;
    }
}

bool greater(const Z8Number& x, const Z8Number& y) {
    if (x == y) return false;
    if (y == Z8Number("a")) return true;
    if (x == Z8Number("a")) return false;
    try {
        x - y;
        return true;
    }
    catch (...) {
        return false;
    }
}

bool greaterOrEqual(const Z8Number& X, const Z8Number& Y) {
    std::string x = X.digits;
    std::string y = Y.digits;
    return Z8Number::greaterOrEqual(x, y);
}

std::string Z8Number::addNumbers(const std::string& x, const std::string& y) {
    if (isEqual(y, "a")) return x;
    std::string res = x;
    std::string counter = "a";
    while (!isEqual(counter, y)) {
        res = incNumber(res);
        counter = incNumber(counter);
        if (normalize(res).size() > 8)
            throw std::overflow_error("Number exceeds 8 digits");
    }
    return res;
}

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

std::string Z8Number::mulNumbers(const std::string& x, const std::string& y) {
    if (isEqual(x, "a") || isEqual(y, "a")) return "a";
    std::string res = x;
    std::string counter = "b";
    if (isEqual(y, "ba")) res = res + "a";
    else if (isEqual(y, "baa")) res = res + "aa";
    else if (isEqual(y, "baaa")) res = res + "aaa";
    else if (isEqual(y, "baaaa")) res = res + "aaaa";
    else if (isEqual(y, "baaaaa")) res = res + "aaaaa";
    else if (isEqual(y, "baaaaaa")) res = res + "aaaaaaa";
    else if (isEqual(y, "baaaaaaa")) res = res + "aaaaaaa";
    else {
        while (!isEqual(counter, y)) {
            res = addNumbers(res, x);
            counter = incNumber(counter);
        }
    }
    if (normalize(res).size() > 8)
        throw std::overflow_error("Number exceeds 8 digits");
    return res;
}

std::string Z8Number::divNumbers(const std::string& x, const std::string& y, bool firstIsNeg) {
    if (isEqual(y, "a")) {
        if (isEqual(x, "a"))
            return "[-ffffffff; ffffffff]";
        else
            return "empty set";
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

    if (firstIsNeg && !isEqual(remainder, "a")) {
        quotient = incNumber(quotient);
        remainder = subNumbers(y, remainder);
    }

    if (isEqual(remainder, "a")) {
        return quotient;
    }
    else {
        return quotient + "(" + remainder + ")";
    }
}

std::string Z8Number::divide(const Z8Number& divisor, bool firstIsNeg) const {
    return divNumbers(digits, divisor.digits, firstIsNeg);
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
    std::string result;
    if (!(a.isNegative) && (!(b.isNegative))) {
        try {
            if (op == "+") {
                result = (a + b).toString();
            }
            else if (op == "-") {
                try {
                    result = (a - b).toString();
                }
                catch (...) {
                    result = "-" + (b - a).toString();
                }
            }
            else if (op == "*") {
                if (greater(a, b)) result = (a * b).toString();
                else result = (b * a).toString();
            }
            else if (op == "/") { 
                result = a.divide(b);
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
        Z8Number neg, pos;
        if ((a.isNegative) && (!(b.isNegative))) {
            neg = a;
            pos = b;
        }
        else {
            neg = b;
            pos = a;
        }
        try {
            if (op == "+") {
                if (greater(neg, pos)) result = "-" + (neg - pos).toString();
                else result = (pos - neg).toString();
            }
            else if (op == "-") {
                result = (neg + pos).toString();
            }
            else if (op == "*") {
                if (a == Z8Number("a") || a == Z8Number("-a") || b == Z8Number("a") || b == Z8Number("-a")) result = "a";
                else result = "-" + (a * b).toString();
            }
            else if (op == "/") {
                if (a == Z8Number("a")) result = a.divide(b);
                else if (a.isNegative) result = "-" + a.divide(b, true);
                else result = "-" + a.divide(b);
            }
            else {
                std::cerr << "Unknown operator\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    else {
        try {
            if (op == "+") {
                if ((a == Z8Number("a") || a == Z8Number("-a")) && (b == Z8Number("a") || b == Z8Number("-a"))) result = "a";
                else result = "-" + (a + b).toString();
            }
            else if (op == "-") {
                if (a == b) result = "a";
                else if ((a == Z8Number("a") || a == Z8Number("-a")) && (b == Z8Number("a") || b == Z8Number("-a"))) result = "a";
                else {
                    try {
                        result = "-" + (a - b).toString();
                    }
                    catch (...) {
                        result = (b - a).toString();
                    }
                }
            }
            else if (op == "*") {
                result = (a * b).toString();
            }
            else if (op == "/") {
                result = a.divide(b);
            }
            else {
                std::cerr << "Unknown operator\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    if (result == "-empty set") result = "empty set";
    std::cout << result << std::endl;
}