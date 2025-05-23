#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

bool isValidRegister(const std::string &_register);
std::string intToHex(int number);
void printTargetTable(const std::vector<std::pair<std::string, std::string>>& Target_Table);
std::vector<uint8_t> hexStringToBytes(const std::string& hex);


#endif