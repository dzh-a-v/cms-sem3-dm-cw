#pragma once

#include <string>
#include <map>

// Одноразрядные операции
char add(char x, char y);
char sub(char x, char y);

// Многоразрядные операции (макс. 8 разрядов, старший — слева)
std::string addStrings(const std::string& x, const std::string& y);
std::string subStrings(const std::string& x, const std::string& y);

// Вспомогательные функции (для внутреннего использования)
char succ(char x);
char pred(char x);