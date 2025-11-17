#pragma once

#include <string>
#include <map>

extern const long long MOD;

extern std::map<char, int> elemToInt;
extern std::map<int, char> intToElem;

int toInt(char c);
char toElem(int n);
long long stringToNumber(const std::string& s);
std::string numberToString(long long num);
long long normalize(long long x);

std::string addStr(const std::string& x, const std::string& y);
std::string subStr(const std::string& x, const std::string& y);
std::string mulStr(const std::string& x, const std::string& y);
std::string divStr(const std::string& x, const std::string& y);

long long gcdLong(long long a, long long b);
long long lcmLong(long long a, long long b);