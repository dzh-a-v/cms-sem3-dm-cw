// main.cpp
#include <iostream>
#include <string>
#include "z8.h"

int main() {
    std::cout << "=== Test ===\n\n";

    // Одноразрядные операции
    std::cout << "b + d = " << add('b', 'd') << " (expecting h)\n";
    std::cout << "h - d = " << sub('h', 'd') << " (expecting b)\n";
    std::cout << "a - b = " << sub('a', 'b') << " (expecting f)\n\n";

    // Многоразрядные
    std::cout << "bbb + ccc = " << addStrings("bbb", "ccc") << "\n";
    std::cout << "a - b = " << subStrings("a", "b") << " (expecting fffffff)\n";
    std::cout << "bb - b = " << subStrings("bb", "b") << "\n";
    std::cout << "a - a = " << subStrings("a", "a") << "\n";

    return 0;
}