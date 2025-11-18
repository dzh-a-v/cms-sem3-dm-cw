// z8.cpp
#include "z8.h"
#include <stdexcept>
#include <algorithm>
#include <vector>

// Глобальные таблицы (инициализируются один раз)
static std::map<char, std::map<char, char>> addTable;
static std::map<char, char> inverseMap;
static std::vector<char> chain;
static std::map<char, int> order;

// Правило +1 для варианта 18
char succ(char x) {
    switch (x) {
    case 'a': return 'b';
    case 'b': return 'd';
    case 'c': return 'e';
    case 'd': return 'h';
    case 'e': return 'g';
    case 'f': return 'a';
    case 'g': return 'f';
    case 'h': return 'c';
    default: throw std::invalid_argument("Invalid element in succ()");
    }
}

// Обратное правило (для построения цепочки)
char pred(char x) {
    switch (x) {
    case 'a': return 'f';
    case 'b': return 'a';
    case 'c': return 'h';
    case 'd': return 'b';
    case 'e': return 'c';
    case 'f': return 'g';
    case 'g': return 'e';
    case 'h': return 'd';
    default: throw std::invalid_argument("Invalid element in pred()");
    }
}

// Инициализация структур (вызывается один раз)
static void initZ8() {
    static bool initialized = false;
    if (initialized) return;

    // Построим аддитивную цепочку от 'a'
    chain = { 'a', 'b', 'd', 'h', 'c', 'e', 'g', 'f' };
    for (int i = 0; i < 8; ++i) {
        order[chain[i]] = i;
    }

    // Построим таблицу сложения
    for (char x : chain) {
        for (char y : chain) {
            int sum = (order[x] + order[y]) % 8;
            addTable[x][y] = chain[sum];
        }
    }

    // Построим таблицу обратных элементов (для вычитания)
    for (char x : chain) {
        for (char y : chain) {
            if (addTable[x][y] == 'a') {
                inverseMap[x] = y;
                break;
            }
        }
    }

    initialized = true;
}

// Одноразрядное сложение
char add(char x, char y) {
    initZ8();
    return addTable.at(x).at(y);
}

// Одноразрядное вычитание: x - y = x + (-y)
char sub(char x, char y) {
    initZ8();
    char invY = inverseMap.at(y);
    return addTable.at(x).at(invY);
}

// Выравнивание строк до одинаковой длины (добавляем 'a' слева)
static std::pair<std::string, std::string> align(const std::string& x, const std::string& y) {
    size_t maxLen = std::max(x.size(), y.size());
    maxLen = std::min(maxLen, size_t(8)); // не более 8 разрядов

    std::string xPad = std::string(maxLen - x.size(), 'a') + x;
    std::string yPad = std::string(maxLen - y.size(), 'a') + y;
    return { xPad, yPad };
}

// Многоразрядное сложение (старший разряд слева)
std::string addStrings(const std::string& x, const std::string& y) {
    initZ8();
    if (x.empty() || y.empty()) throw std::invalid_argument("Empty input");

    auto [xPad, yPad] = align(x, y);
    std::string result = "";
    char carry = 'a'; // начальный перенос = 0

    // Идём справа налево (младший разряд — последний)
    for (int i = xPad.size() - 1; i >= 0; --i) {
        char sum1 = add(xPad[i], yPad[i]);
        char total = add(sum1, carry);

        // Определяем, был ли перенос:
        int sumVal = order[xPad[i]] + order[yPad[i]] + order[carry];
        carry = (sumVal >= 8) ? 'b' : 'a'; // 'b' = 1

        result = total + result;
    }

    // Если остался перенос — добавляем (но не более 8 разрядов)
    if (carry != 'a') {
        result = carry + result;
        if (result.size() > 8) {
            result = result.substr(result.size() - 8); // усечение до 8 разрядов
        }
    }

    // Убираем ведущие 'a', но оставляем хотя бы один символ
    size_t start = result.find_first_not_of('a');
    if (start == std::string::npos) return "a";
    return result.substr(start);
}

// Многоразрядное вычитание
std::string subStrings(const std::string& x, const std::string& y) {
    initZ8();
    if (x.empty() || y.empty()) throw std::invalid_argument("Empty input");

    auto [xPad, yPad] = align(x, y);
    std::string result = "";
    char borrow = 'a'; // заём = 0

    for (int i = xPad.size() - 1; i >= 0; --i) {
        // x_i - borrow
        char xb = (borrow == 'a') ? xPad[i] : sub(xPad[i], borrow);
        // xb - y_i
        char diff = sub(xb, yPad[i]);

        // Нужен ли заём?
        int xi = order[xPad[i]];
        int yi = order[yPad[i]];
        int bi = order[borrow];

        // Если (xi - bi) < yi → нужен заём
        if (xi - bi < yi) {
            borrow = 'b';
        }
        else {
            borrow = 'a';
        }

        result = diff + result;
    }

    // Убираем ведущие 'a'
    size_t start = result.find_first_not_of('a');
    if (start == std::string::npos) return "a";
    return result.substr(start);
}