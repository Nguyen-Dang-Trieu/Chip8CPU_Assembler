#include "instructionnode.h"
#include "literalnode.h"
#include "rootnode.h"
#include "symbolnode.h"
#include "statement.h"
#include "utils_.h"
#include "assembler.h"


std::string replace_extension(const std::string &filename, const std::string &new_ext)
{
    size_t dot_pos = filename.find_last_of(".");
    if (dot_pos == std::string::npos)
    {
        return filename + new_ext;
    }
    return filename.substr(0, dot_pos) + new_ext;
}

int main(int argc, char* argv[])
{
    // Khởi tạo Assembler
    AssemblerChip8 Init;


    // Xử lí các đối số đầu vào
    if (argc < 2)
    {
        std::cerr << "Error: Missing input assembly (.asm) file.\n";
        std::cerr << "Usage: " << argv[0] << " <input_file>.asm\n";
        return 1;
    }
    std::string file_input = argv[1];
    std::string file_output = replace_extension(file_input, ".ch8");

    std::ifstream input("program\\" + file_input);
    std::ofstream output(file_output, std::ios::binary);

    // Kiểm tra xem file có mở thành công hay không
    if (!input.is_open() || !output.is_open())
    {
        std::cerr << "Cannot open file!" << std::endl;
        return 1;
    }

    // Đọc toàn bộ nội dung của file vào 1 chuỗi
    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string source = buffer.str();

    // ------------------------------- PASS - 1 ---------------------------------------
    ProgramNode *output_pass_1 = Init.pass_1(source);
    output_pass_1->print();

    // Đóng file sau khi đọc xong
    input.close();

    // ------------------------------- PASS - 2 ----------------------------------------
    std::vector<std::pair<std::string, std::string>> output_pass_2 = Init.pass_2(output_pass_1);

    // Hiển thị Target Table
    printTargetTable(output_pass_2);

    // Xuất ra file nhị phân
    // B1: tách opcode
    std::vector<uint8_t> opcodes_file;
    for (const auto &entry : output_pass_2)
    {
        std::vector<uint8_t> bytes = hexStringToBytes(entry.second);
        opcodes_file.insert(opcodes_file.end(), bytes.begin(), bytes.end());
    }

    // B2: Ghi vào file nhị phân
    output.write(reinterpret_cast<char *>(opcodes_file.data()), opcodes_file.size());
    output.close();

    std::cout << "Binary file created successfully" << std::endl;

    return 0;
}

