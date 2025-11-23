#include <iostream>
#include <string>
#include "z8.h"

int main() {
    std::string s1, s2, op;
    while (true) {
        std::cout << "Num1: "; std::cin >> s1;
        std::cout << "Op (+, -, *, /): "; std::cin >> op;
        std::cout << "Num2: "; std::cin >> s2;

        try {
            Z8Number a(s1), b(s2);
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
}