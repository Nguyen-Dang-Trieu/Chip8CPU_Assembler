#ifndef ASSEMBLER_H
#define ASSEMBLER_h

class AssemblerChip8
{
public:
    ProgramNode *parser(const std::string &source);

    ProgramNode *pass_1(const std::string &source);
    std::vector<std::pair<std::string, std::string>> pass_2(ProgramNode *pass_1);
};

// Hàm phân tích cú pháp mã nguồn assembler
ProgramNode *AssemblerChip8::parser(const std::string &source)
{
    uint16_t labelAddress = 0x200;
    ProgramNode *program = new ProgramNode();
    std::stringstream ss(source); // chứa toàn bộ source code trong file asm

    std::string line;
    while (std::getline(ss, line))
    {
        std::stringstream line_ss(line);
        std::string token; // token ở đây là mỗi thành phần của instruction: mnemonic, label, literal
        std::string opcode;
        line_ss >> token;

        if (token.back() == ':')
        {
            // Label:
            std::string symbol_name = token.substr(0, token.size() - 1);
            program->addLabel(new SymbolNode(symbol_name, labelAddress));
        }
        else
        {
            // Opcode của Instruction
            std::string delete_Space = line_ss.str();
            delete_Space.erase(0, delete_Space.find_first_not_of(" \t")); // Xóa tất cả khoảng trắng đầu
            delete_Space.erase(delete_Space.find_last_not_of(" \t") + 1); // Xóa tất cả khoảng trắng cuôis
            opcode = detectOpcode(delete_Space);

            // Instruction
            std::string mnemonic = token;
            std::vector<std::string> registers;
            std::string symbol_;
            std::string literal_;

            while (line_ss >> token)
            {
                // Thêm vào bảng Literal Table
                if (std::isdigit(token[0]) || (token[0] == '0' && token[1] == 'x'))
                {
                    literal_ = token; // Để thêm vào trong instruction

                    // Hằng số: giá trị nằm trong khoảng 0->255
                    try
                    {
                        uint8_t value = static_cast<uint8_t>(std::stoul(token, nullptr, 0));
                        program->addLiteral(new LiteralNode(value));
                    }
                    // Xử lí ngoại lệ của hàm stoul
                    catch (const std::invalid_argument &err)
                    {
                        std::cerr << "Error: Invalid literal: " << token << std::endl;
                        break;
                    }
                    catch (const std::out_of_range &err)
                    {
                        std::cerr << "Error: Literal out of range: " << token << std::endl;
                        break;
                    }
                }
                else // Xử lí các thanh ghi + symbol
                {
                    if (!token.empty() && token.front() == 'V') // Thanh ghi
                    {
                        std::string reg = token;
                        if (token.back() == ',') // kiểm tra chuỗi có rỗng hay hk, và loại bỏ dấu ,
                        {
                            reg = token.substr(0, token.size() - 1);
                        }

                        if (isValidRegister(reg))
                        {
                            registers.push_back(reg);
                        }
                        else
                        {
                            std::cerr << "Error: Invalid register: " << token << std::endl;
                        }
                    }
                    else // Symbol
                    {
                        symbol_ = token;
                    }
                }
            }

            program->addInstruction(new InstructionNode(mnemonic, registers, symbol_, literal_, opcode));

            labelAddress += 2;
        }
    }

    return program;
}

// ------------------------------------------- PASS - 1 ------------------------------------------------
ProgramNode *AssemblerChip8::pass_1(const std::string &source)
{
    return parser(source);
}

// ------------------------------------------- PASS - 2 ------------------------------------------------
std::vector<std::pair<std::string, std::string>> AssemblerChip8::pass_2(ProgramNode *pass_1)
{
    std::string x;   // Vx
    std::string y;   // Vy
    std::string kk;  // byte
    std::string nnn; // addr
    std::string opcode_actual;
    std::vector<std::pair<std::string, std::string>> Target_Table;
    std::vector<InstructionNode *> Instruction_Table = pass_1->get_InstructionTale();
    std::vector<SymbolNode *> Symbol_Table = pass_1->get_SymbolTable();

    // LC: Location Counter
    uint16_t LC = 512; // 512 = 0x200 là địa chỉ bắt đầu lưu opcode

    for (size_t i = 0; i < Instruction_Table.size(); i++)
    {
        opcode_actual = Instruction_Table[i]->get_Opcode();

        // Xử lí thanh ghi
        std::vector<std::string> Register = Instruction_Table[i]->get_registers();
        if (!Register.empty()) // kiểm tra thử có thanh ghi hay không ?
        {
            if (opcode_actual.find('x', 0u) != std::string::npos) // nếu có thanh ghi Vx
            {
                opcode_actual[1] = Register[0][1]; // Thanh ghi Vx
            }

            if (opcode_actual.find('y', 0u) != std::string::npos)
            {
                opcode_actual[2] = Register[1][1]; // Thanh ghi Vy
            }
        }

        // Xử lí giá trị
        if (opcode_actual.find('k', 0u) != std::string::npos)
        {
            std::string value = Instruction_Table[i]->get_literal().substr(2);
            opcode_actual.replace(2, 2, value);
        }

        // Xử lí địa chỉ
        if (opcode_actual.find('n', 0u) != std::string::npos)
        {
            std::string symbol = Instruction_Table[i]->get_symbol();
            std::string address = "";

            for (size_t j = 0; j < Symbol_Table.size(); j++)
            {
                if (Symbol_Table[j]->get_name_symbol() == symbol)
                {
                    address = Symbol_Table[j]->get_address_hex().substr(2);
                    break; // thoát vòng lặp khi tìm thấy symbol
                }
            }

            if (!address.empty())
                opcode_actual.replace(1, 3, address);
        }

        // Thêm vào Target table
        std::string address = intToHex(LC);
        Target_Table.push_back(std::make_pair(address, opcode_actual));
        LC += 2;
    }

    return Target_Table;
}

#endif