#ifndef LITERALNODE_H
#define LITERALNODE_H

#include "ASTnode.h"

class LiteralNode : public ASTNode
{
private:
    int address;
    uint16_t value_dec;
    std::string value_hex;

public:
    LiteralNode(uint16_t value) : value_dec(value)
    {
        std::stringstream hexValue;
        hexValue << "0x" << std::hex << std::uppercase
                 << std::setw(2) << std::setfill('0')
                 << (value_dec & 0xFF); 
                 
        value_hex = hexValue.str();
    }

    std::string get_valueHex()
    {
        return value_hex;
    }

    void print() const override
    {
        std::cout << "| " << std::setw(10) << std::left << value_hex
                  << " |  None      |" << std::endl;
    }
};

#endif