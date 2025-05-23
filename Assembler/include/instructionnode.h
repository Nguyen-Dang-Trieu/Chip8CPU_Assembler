#ifndef INSTRUCTIONNODE_H
#define INSTRUCTIONNODE_H

#include "ASTnode.h"


class InstructionNode : public ASTNode
{
private: // instruction gồm 3 thành phần: mnemonic, register, literal
    std::string mnemonic_;
    std::vector<std::string> registers_;
    std::string symbol_;
    std::string literal_;
    std::string opcode_;

public:
    InstructionNode(const std::string &mnemonic, const std::vector<std::string> registers, std::string symbol,
                    std::string literal, std::string opcode)
        : mnemonic_(mnemonic), registers_(registers), symbol_(symbol), literal_(literal), opcode_(opcode) {}

    std::vector<std::string> get_registers()
    {
        return registers_;
    }

    std::string get_symbol()
    {
        return symbol_;
    }

    std::string get_literal()
    {
        return literal_;
    }

    std::string get_Opcode()
    {
        return opcode_;
    }

    void print() const override
    {
        auto join = [](const std::vector<std::string> &vec, const std::string &delimiter) -> std::string
        {
            std::ostringstream oss;
            for (size_t i = 0; i < vec.size(); ++i)
            {
                if (i > 0)
                    oss << delimiter;
                oss << vec[i];
            }
            return oss.str();
        };

        std::cout << "| " << std::setw(10) << std::left << opcode_ << " | "
                  << std::setw(10) << std::left << mnemonic_ << " | "
                  << std::setw(10) << std::left << join(registers_, ", ") << " | "
                  << std::setw(10) << std::left << symbol_ << " | "
                  << std::setw(10) << std::left << literal_ << " |"
                  << std::endl;
    }
};


#endif