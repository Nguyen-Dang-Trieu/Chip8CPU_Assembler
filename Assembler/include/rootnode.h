#ifndef ROOTNODE_H
#define ROOTNODE_H

#include "symbolnode.h"
#include "literalnode.h"
#include "instructionnode.h"

// Root node
class ProgramNode : public ASTNode
{
private:
    std::vector<SymbolNode *> labels_;
    std::vector<InstructionNode *> instructions_;
    std::vector<LiteralNode *> literals_;

public:
    void addLabel(SymbolNode *label)
    {
        labels_.push_back(label);
    }

    void addInstruction(InstructionNode *instruction)
    {
        instructions_.push_back(instruction);
    }

    void addLiteral(LiteralNode *literal)
    {
        literals_.push_back(literal);
    }

    std::vector<SymbolNode *> get_SymbolTable()
    {
        return labels_;
    }

    std::vector<InstructionNode *> get_InstructionTale()
    {
        return instructions_;
    }

    std::vector<LiteralNode *> get_LiteralTable()
    {
        return literals_;
    }

    void print() const override
    {
        // ------------- Symbol Table ----------------
        std::cout << "Symbol Table\n";
        std::cout << "+------------+------------+" << std::endl;
        std::cout << "|   Symbol   |   Address  |" << std::endl;
        std::cout << "+------------+------------+" << std::endl;

        for (const auto &label : labels_)
        {
            label->print();
        }
        std::cout << "+------------+------------+\n";

        // ------------- Instruction Table -------------
        std::cout << "Instruction Table\n";
        std::cout << "+------------+------------+------------+------------+------------+" << std::endl;
        std::cout << "|   Opcode   |  Mnemonic  | Registers  |   Symbol   |   Literal  |" << std::endl;
        std::cout << "+------------+------------+------------+------------+------------+" << std::endl;
        for (const auto &instruction : instructions_)
        {
            instruction->print();
        }
        std::cout << "+------------+------------+------------+------------+------------+\n";

        // ----------------- Literal Table ---------------------
        std::cout << "\nLiteral Table:\n";
        std::cout << "+------------+------------+" << std::endl;
        std::cout << "|   Literal  |   Address  |" << std::endl;
        std::cout << "+------------+------------+" << std::endl;
        for (const auto &literal : literals_)
        {
            literal->print();
        }
        std::cout << "+------------+------------+\n";
    }
};


#endif