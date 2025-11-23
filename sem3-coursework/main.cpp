#include <iostream>
#include <string>
#include "z8.h"

int main() {
    std::string s1, s2, op;
    std::cout << "Num1: "; std::cin >> s1;
    std::cout << "Op (+, -, *): "; std::cin >> op;
    std::cout << "Num2: "; std::cin >> s2;

    try {
        Z8Number a(s1), b(s2);
        Z8Number res = (op == "+") ? a + b :
            (op == "-") ? a - b :
            (op == "*") ? a * b :
            (throw std::invalid_argument("Bad op"), Z8Number());
        std::cout << "Result: " << res.toString() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}