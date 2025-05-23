#include "utils_.h"

bool isValidRegister(const std::string &_register)
{
    if (_register.size() != 2 || _register.at(0) != 'V')
        return false;

    if (std::isdigit(_register.at(1)) || (_register.at(1) >= 'A' && _register.at(1) <= 'F'))
        return true;

    return false;
}


std::string intToHex(int number)
{
    std::stringstream ss;
    ss << "0x" << std::hex << number;
    return ss.str();
}

void printTargetTable(const std::vector<std::pair<std::string, std::string>>& Target_Table)
{
    const int col_width = 12; 

    std::cout << "Target Table\n";
    std::cout << "+------------+------------+" << std::endl;
    std::cout << "|   Address  |   Opcode   |" << std::endl;
    std::cout << "+------------+------------+" << std::endl;

    for (const auto& entry : Target_Table)
    {

        int space_left1 = (col_width - entry.first.length()) / 2;
        int space_right1 = col_width - entry.first.length() - space_left1;

        int space_left2 = (col_width - entry.second.length()) / 2;
        int space_right2 = col_width - entry.second.length() - space_left2;

        std::cout << "|"
                  << std::string(space_left1, ' ') << entry.first << std::string(space_right1, ' ')
                  << "|"
                  << std::string(space_left2, ' ') << entry.second << std::string(space_right2, ' ')
                  << "|" << std::endl;
    }

    std::cout << "+------------+------------+\n";
}

std::vector<uint8_t> hexStringToBytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));
        bytes.push_back(byte);
    }
    return bytes;
}