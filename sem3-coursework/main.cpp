#include <iostream>
#include <string>
#include <exception>
#include "z8calc.h"

void printHelp() {
    std::cout << "\nMulti-digit Calculator (Z8, variant 18)\n";
    std::cout << "Each number is a string of digits from {a,b,c,d,e,f,g,h}\n";
    std::cout << "Example: 'bbb' = 1*8^2 + 1*8 + 1 = 73\n\n";
    std::cout << "Commands:\n"
        << "  + X Y   : X + Y\n"
        << "  - X Y   : X - Y (X >= Y)\n"
        << "  * X Y   : X * Y\n"
        << "  / X Y   : X / Y\n"
        << "  gcd X Y : GCD(X, Y)\n"
        << "  lcm X Y : LCM(X, Y)\n"
        << "  q       : quit\n\n";
}

int main() {
    printHelp();

    std::string op;
    while (std::cin >> op) {
        if (op == "q") break;

        try {
            if (op == "+" || op == "-" || op == "*" || op == "/") {
                std::string x, y;
                std::cin >> x >> y;
                if (x.length() > 8 || y.length() > 8) {
                    std::cout << "Number can only have 8 digits. Try again.\n";
                    continue;
                }
                std::string res;
                if (op == "+") res = addStr(x, y);
                else if (op == "-") res = subStr(x, y);
                else if (op == "*") res = mulStr(x, y);
                else if (op == "/") res = divStr(x, y);
                std::cout << "= " << res << "\n";
            }
            else if (op == "gcd" || op == "lcm") {
                std::string x, y;
                std::cin >> x >> y;
                if (x.length() > 8 || y.length() > 8) {
                    std::cout << "Number can only have 8 digits. Try again.\n";
                    continue;
                }
                long long a = stringToNumber(x), b = stringToNumber(y);
                long long res_val = (op == "gcd") ? gcdLong(a, b) : lcmLong(a, b);
                if (res_val != -1) {
                    if (res_val >= MOD)
                        res_val = normalize(res_val);
                    std::string res_str = numberToString(res_val);
                    std::cout << op << "(" << x << ", " << y << ") = " << res_str << "\n";
                }
            }
            else {
                std::cout << "Unknown command.\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Err0r: " << e.what() << "\n";
        }
    }

    std::cout << "Bye!\n";
    return 0;
}