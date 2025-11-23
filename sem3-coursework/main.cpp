#include "z8.h"
#include <iostream>
#include <string>
#include <cctype>

int main() {
    std::cout << "=== Z8 Finite Arithmetic Calculator ===\n";
    std::cout << "Operations: +  -  *  /\n";
    std::cout << "Format: <num1> <op> <num2>   (e.g., bbb + ccc)\n";
    std::cout << "Digits: 'a' to 'h' (1–8 digits)\n";
    std::cout << "Type 'exit' to quit.\n\n";

    std::string line;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "exit") break;
        if (line.empty()) continue;

        std::string num1_str, num2_str;
        char op;
        if (!ParseExpression(line, num1_str, op, num2_str)) {
            std::cout << "Invalid format...\n\n";
            continue;
        }

        if (!IsValidNumber(num1_str) || !IsValidNumber(num2_str)) {
            std::cout << "Invalid digits...\n\n";
            continue;
        }

        Z8Number a(num1_str), b(num2_str);
        try {
            if (op == '+') {
                std::cout << "Result: "; print_number((a + b).str()); std::cout << "\n\n";
            }
            else if (op == '-') {
                std::cout << "Result: "; print_number((a - b).str()); std::cout << "\n\n";
            }
            else if (op == '*') {
                std::cout << "Result: "; print_number((a * b).str()); std::cout << "\n\n";
            }
            else if (op == '/') {
                auto [q, r] = a / b;
                std::string qs = q.str();
                if (qs == "all") {
                    std::cout << "Result: 0/0 — any value.\n\n";
                }
                else if (qs == "no") {
                    std::cout << "Result: Division by zero.\n\n";
                }
                else {
                    std::cout << "Result: "; print_number(qs);
                    std::cout << "\nRemainder: "; print_number(r.str());
                    std::cout << "\n\n";
                }
            }
        }
        catch (...) {
            std::cout << "Calculation error.\n\n";
        }
    }
    std::cout << "Goodbye!\n";
    return 0;
}