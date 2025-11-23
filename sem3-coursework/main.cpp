#include <iostream>
#include <string>
#include "z8.h"

int main() {
    std::string s1, s2, op;
    while (true) {
        std::cout << "=== DEBUG ===" << std::endl;
        std::cout << "a + b = " << (Z8Number("a") + Z8Number("b")).toString() << "\n";
        std::cout << "f + b = " << (Z8Number("f") + Z8Number("b")).toString() << "\n";
        std::cout << "a * d = " << (Z8Number("a") * Z8Number("d")).toString() << "\n";
        std::cout << "ff * f = " << (Z8Number("ff") + Z8Number("f")).toString() << "\n";
        std::cout << "aaab * c = " << (Z8Number("aaab") + Z8Number("c")).toString() << "\n";
        std::cout << "f - b = " << (Z8Number("f") - Z8Number("b")).toString() << "\n";
        std::cout << "ba - d = " << (Z8Number("ba") - Z8Number("d")).toString() << "\n";
        std::cout << "ba / c = " << Z8Number("ba").divide(Z8Number("c")) << "\n";
        std::cout << "-ba + b = "; calculate(Z8Number("-ba"), Z8Number("b"), "+");
        std::cout << "-b + f = "; calculate(Z8Number("-b"), Z8Number("f"), "+");
        std::cout << "-ba - -ff = "; calculate(Z8Number("-ba"), Z8Number("-ff"), "-");
        std::cout << "b - d = "; calculate(Z8Number("b"), Z8Number("d"), "-");
        std::cout << "-a + a = "; calculate(Z8Number("-a"), Z8Number("a"), "+");
        std::cout << "Num1: "; std::cin >> s1;
        std::cout << "Op (+, -, *, /): "; std::cin >> op;
        std::cout << "Num2: "; std::cin >> s2;

        Z8Number a(s1), b(s2);
        calculate(a, b, op);
    }
}