#ifndef SYMBOLNODE_H
#define SYMBOLNODE_H

#include "ASTnode.h"


class SymbolNode : public ASTNode
{
private:
    std::string _name;
    uint16_t _address_dec;    // Địa chỉ dạng thập phân
    std::string _address_hex; // Địa chỉ dạng hexa

public:
    SymbolNode(const std::string &name, uint16_t address_dec)
        : _name(name), _address_dec(address_dec)
    {
        std::stringstream hexAddress;
        hexAddress << "0x" << std::hex << std::uppercase
                   << std::setw(3) << std::setfill('0')
                   << (_address_dec & 0xFFF); // Giữ 3 chữ số hex cuối

        // Lưu địa chỉ dưới dạng string
        _address_hex = hexAddress.str();
    }

    std::string get_name_symbol()
    {
        return _name;
    }

    uint16_t get_address_dec()
    {
        return _address_dec;
    }

    std::string get_address_hex()
    {
        return _address_hex;
    }

    void print() const override
    {
        std::cout << "| " << std::setw(10) << std::left << _name
                  << " | " << std::setw(7) << std::right << _address_hex
                  << "    |" << std::endl;
    }
};

#endif