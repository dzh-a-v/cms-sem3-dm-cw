#include <iostream>
#include <string>
#include "z8.h"

int main() {
    std::cout << "Hello!" << std::endl;
    std::string s1, s2, op;
    /*std::cout << "=== DEBUG ===" << std::endl;
    std::cout << "a + b = " << (Z8Number("a") + Z8Number("b")).toString() << "\n";
    std::cout << "f + b = " << (Z8Number("f") + Z8Number("b")).toString() << "\n";
    std::cout << "a * d = " << (Z8Number("a") * Z8Number("d")).toString() << "\n";
    std::cout << "ff * f = " << (Z8Number("ff") + Z8Number("f")).toString() << "\n";
    std::cout << "aaab * c = " << (Z8Number("aaab") + Z8Number("c")).toString() << "\n";
    std::cout << "f - b = " << (Z8Number("f") - Z8Number("b")).toString() << "\n";
    std::cout << "ba - d = " << (Z8Number("ba") - Z8Number("d")).toString() << "\n";
    std::cout << "e + e = " << (Z8Number("e") + Z8Number("e")).toString() << "\n";
    std::cout << "ba / e = " << Z8Number("ba").divide(Z8Number("e")) << "\n";
    std::cout << "-ba + b = "; calculate(Z8Number("-ba"), Z8Number("b"), "+");
    std::cout << "-b + f = "; calculate(Z8Number("-b"), Z8Number("f"), "+");
    std::cout << "-ba - -ff = "; calculate(Z8Number("-ba"), Z8Number("-ff"), "-");
    std::cout << "b - d = "; calculate(Z8Number("b"), Z8Number("d"), "-");
    std::cout << "-a + a = "; calculate(Z8Number("-a"), Z8Number("a"), "+");
    std::cout << "ba * baa = "; calculate(Z8Number("ba"), Z8Number("baa"), "*");
    std::cout << "-a - -a ="; calculate(Z8Number("-a"), Z8Number("-a"), "-");
    std::cout << "-a + -a ="; calculate(Z8Number("-a"), Z8Number("-a"), "+");
    std::cout << "-a * a ="; calculate(Z8Number("-a"), Z8Number("a"), "*");
    std::cout << "-a * baa ="; calculate(Z8Number("-a"), Z8Number("baa"), "*");*/
    while (true) {
        std::cout << "Enter the problem (format: <num> <operation> <num>; nums from -ffffffff to ffffffff):\n>> ";
        std::cin >> s1;
        std::cin >> op;
        std::cin >> s2;

        try {
            Z8Number a(s1), b(s2);
            calculate(a, b, op);
        }
        catch (...) {
            std::cerr << "Error: incorrect input" << std::endl;
        }
    }
}