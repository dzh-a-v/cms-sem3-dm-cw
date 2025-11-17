#include "z8calc.h"
#include <stdexcept>
#include <cmath>
#include <iostream>

const long long MOD = 16777216LL; // 8^8

std::map<char, int> elemToInt = {
    {'a', 0}, {'b', 1}, {'d', 2}, {'h', 3},
    {'c', 4}, {'e', 5}, {'g', 6}, {'f', 7}
};

std::map<int, char> intToElem = {
    {0, 'a'}, {1, 'b'}, {2, 'd'}, {3, 'h'},
    {4, 'c'}, {5, 'e'}, {6, 'g'}, {7, 'f'}
};

int toInt(char c) {
    /*if (elemToInt.count(c) == 0)
        throw std::invalid_argument("Invalid element: " + std::string(1, c));*/
    return elemToInt[c];
}

char toElem(int n) {
    //n = (n % 8 + 8) % 8;
    return intToElem[n];
}

long long stringToNumber(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("Empty number");
    /*for (char c : s)
        if (elemToInt.count(c) == 0)
            throw std::invalid_argument("Invalid digit in number: " + std::string(1, c));*/
    long long num = 0;
    for (char c : s) {
        num = num * 8 + toInt(c);
    }
    return num;
}

std::string numberToString(long long num) {
    /*if (num < 0 || num >= MOD)
        throw std::runtime_error("Number out of 8-digit Z8 range [0, 8^8)");*/
    if (num == 0) return "a";
    std::string res = "";
    while (num > 0) {
        res = toElem(num % 8) + res;
        num /= 8;
    }
    return res;
}

long long normalize(long long x) {
    if (x < 0) x += MOD;
    else if (x > MOD) x %= MOD;
    return x;
}

std::string addStr(const std::string& x, const std::string& y) {
    return numberToString(normalize(stringToNumber(x) + stringToNumber(y)));
}

std::string subStr(const std::string& x, const std::string& y) {
    return numberToString(normalize(stringToNumber(x) - stringToNumber(y)));
}

std::string mulStr(const std::string& x, const std::string& y) {
    return numberToString(normalize(stringToNumber(x) * stringToNumber(y)));
}

std::string divStr(const std::string& x, const std::string& y) {
    long long a = stringToNumber(x);
    long long b = stringToNumber(y);
    if (b == 0)
        throw std::runtime_error("Division by zero");

    long long quotient = a / b;
    long long remainder = a % b;

    if (quotient >= MOD)
        quotient = normalize(quotient);

    if (remainder == 0) {
        return numberToString(quotient);
    }
    else {
        return numberToString(quotient) + "." + numberToString(remainder);
    }
}

long long gcdLong(long long a, long long b) {
    if (a == 0 || b == 0) {
        std::cout << "\nWrong value: 0 cannot be the argument of GCD!\n";
        return -1;
    }
    while (b) {
        long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

long long lcmLong(long long a, long long b) {
    if (a == 0 || b == 0) return 0;
    return (a / gcdLong(a, b)) * b;
}