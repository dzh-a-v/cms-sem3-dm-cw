#include <iostream>
#include <string>
#include "z8.h"

int main() {
    std::string input1, input2, op;
    std::cout << "Enter first number (up to 8 digits, e.g. f): ";
    std::cin >> input1;
    std::cout << "Enter operation (+, -, *): ";
    std::cin >> op;
    std::cout << "Enter second number: ";
    std::cin >> input2;

    try {
        Z8Number a(input1);
        Z8Number b(input2);
        Z8Number res;

        if (op == "+") {
            res = a + b;
        }
        else if (op == "-") {
            res = a - b;
        }
        else if (op == "*") {
            res = a * b;
        }
        else {
            std::cerr << "Unsupported operation\n";
            return 1;
        }

        std::cout << "Result: " << res.toString() << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}