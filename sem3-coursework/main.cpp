#include "z8.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== Z8 Finite Arithmetic Calculator ===\n";
    std::cout << "Operations: +  -  *  /\n";
    std::cout << "Format: <num1> <op> <num2>   (e.g., ddd * ccc)\n";
    std::cout << "Digits: 'a' to 'h' (1–8 digits)\n";
    std::cout << "Max value: ffffffff. Overflow -> 'OVERFLOW'\n";
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
            std::cout << "Invalid format. Use: number operator number\n\n";
            continue;
        }
        if (!IsValidNumber(num1_str) || !IsValidNumber(num2_str)) {
            std::cout << "Numbers must be 1-8 chars from 'a' to 'h'.\n\n";
            continue;
        }

        Z8Number a(num1_str), b(num2_str);
        try {
            if (op == '+') {
                Z8Number r = a + b;
                std::cout << "Result: "; print_number(r.str()); std::cout << "\n\n";
            }
            else if (op == '-') {
                Z8Number r = a - b;
                std::cout << "Result: "; print_number(r.str()); std::cout << "\n\n";
            }
            else if (op == '*') {
                Z8Number r = a * b;
                std::cout << "Result: "; print_number(r.str()); std::cout << "\n\n";
            }
            else if (op == '/') {
                auto [q, rem] = a / b;
                std::string qs = q.str();
                if (qs == "ALL") {
                    std::cout << "Result: 0/0 — any value.\n\n";
                }
                else if (qs == "NO") {
                    std::cout << "Result: Division by zero.\n\n";
                }
                else {
                    std::cout << "Result: "; print_number(qs);
                    std::cout << "\nRemainder: "; print_number(rem.str());
                    std::cout << "\n\n";
                }
            }
        }
        catch (...) {
            std::cout << "Error.\n\n";
        }
    }
    std::cout << "Goodbye!\n";
    return 0;
}