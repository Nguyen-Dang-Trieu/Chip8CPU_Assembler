#include "OpcodeHanlers.h"

// -----------------------------------------  FOR LOOKUP TABLE 1  -----------------------------------------
void Opcode_1nnn(uint16_t opcode, Chip8CPU &cpu)
{
    uint16_t valueRegister = opcode & 0x0FFF;
    cpu.set_PC_Register(valueRegister);
}

void Opcode_2nnn(uint16_t opcode, Chip8CPU &cpu)
{
    uint16_t addr = opcode & 0xFFF;

    // push the address of the next instruction on the stack
    uint16_t PC = cpu.get_PC_Register();
    cpu.pushStack(PC);

    // set the address of the CALL
    cpu.set_PC_Register(addr);
}

void Opcode_3xkk(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t byte = opcode & 0xFF;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    if (valueRegisterVx == byte)
        cpu.set_PC_Register(cpu.get_PC_Register() + 2);
}

void Opcode_4xkk(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t byte = opcode & 0xFF;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    if (valueRegisterVx != byte)
        cpu.set_PC_Register(cpu.get_PC_Register() + 2);
}

void Opcode_5xy0(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);
    uint8_t valueRegisterVy = cpu.get_Registers(Vy);

    if (valueRegisterVx == valueRegisterVy)
        cpu.set_PC_Register(cpu.get_PC_Register() + 2);
}

void Opcode_6xkk(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t highByte_lowNibble = (opcode >> 8) & 0x0F; // lấy 4 bit thấp của high byte
    uint8_t value_register = opcode & 0xFF;

    cpu.set_Registers(highByte_lowNibble, value_register);
}

void Opcode_7xkk(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t byte = opcode & 0xFF;
    uint8_t currentDataRegister = cpu.get_Registers(Vx);
    uint8_t valueRegister = (currentDataRegister + byte) & 0xFF;
    cpu.set_Registers(Vx, valueRegister);
}

void Opcode_9xy0(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;

    if (cpu.get_Registers(Vx) != cpu.get_Registers(Vy))
    {
        cpu.set_PC_Register(cpu.get_PC_Register() + 2);
    }
}

void Opcode_Annn(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t value_register = opcode & 0x0FFF;
    cpu.set_I_Register(value_register);
}

void Opcode_Bnnn(uint16_t opcode, Chip8CPU &cpu)
{
    uint16_t addr = opcode & 0x0FFF;
    cpu.set_PC_Register(addr + cpu.get_Registers(0x00));
}

void Opcode_Cxkk(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t byte = opcode & 0xFF;

    // Handling random numbers
    std::srand(std::time(nullptr));
    uint8_t randomByte = std::rand() % 256;

    // display randomByte on screen for checking
    // std::cout << "randomByte = " << (int)randomByte << std::endl;

    cpu.set_Registers(Vx, byte & randomByte);
}


void Opcode_Dxyn(uint16_t opcode, Chip8CPU &cpu)
{
    // ----- Lấy thông tin đầu vào từ opcode -----
    uint8_t Vx = (opcode >> 8) & 0x0F; // Thanh ghi chứa tọa độ x - column
    uint8_t Vy = (opcode >> 4) & 0x0F; // Thanh ghi chứa tọa độ y - row
    uint8_t nibble = opcode & 0x0F;             // số lượng byte cần đọc

    // ----- Xử lí thông tin -----
    uint8_t Vx_data = cpu.get_Registers(Vx); // Tọa độ x
    uint8_t Vy_data = cpu.get_Registers(Vy); // Tọa độ y

    // Để 16 ở đây là n trong lệnh Dxyn có giá trị từ 0 -> 15
    std::array<uint8_t, 16> sprite; // dùng để chứa dữ liệu đọc được bắt đầu từ địa chỉ được chứa trong thanh ghi I
    std::array<uint8_t, MEMORY_SIZE> memory = cpu.getter_ROM();

    uint16_t I_Register = cpu.get_I_Register(); // 0x200
    for (size_t i = 0; i < nibble; i++)
    {
        sprite[i] = memory[I_Register++];
    }

    // Làm việc với graphics[32][8] và XOR với sprite
    frameBuffer buffer = cpu.get_graphics();

    bool collision = false; // phát hiện va chạm
    for (int row = 0; row < nibble; row++)
    {
        uint8_t spriteByte = sprite[row];
        for (int column = 0; column < 8; column++)
        {
            uint8_t spritePixel = (spriteByte >> (7 - column)) & 0x01;

            // Tọa độ thực trên màn hình (chưa wrap)
            int x_screen = Vx_data + column;
            int y_screen = Vy_data + row;

            int byteIndex = x_screen / 8;
            int bitIndex = 7 - (x_screen % 8);

            if (spritePixel == 1)
            {
                // nếu pixel đang là 1, và XOR = 0 → bị xóa → có va chạm
                if ((buffer[y_screen][byteIndex] >> bitIndex) & 0x01)
                {
                    collision = true;
                }
                // XOR với pixel hiện tại
                buffer[y_screen][byteIndex] ^= (0x01 << bitIndex);
            }
        }
    }

    cpu.set_graphics(buffer);
    cpu.drawFlag = true;

    // set cờ VF nếu có va chạm
    // cpu.V[0xF] = collision ? 1 : 0;
}

// -----------------------------------------  FOR LOOKUP TABLE 2  -----------------------------------------
void Opcode_00E0(uint16_t opcode, Chip8CPU &cpu)
{
    frameBuffer currentDataGraphics = cpu.get_graphics();

    // clear
    for (auto &row : currentDataGraphics)
    {
        std::fill(row.begin(), row.end(), 0);
    }
    cpu.set_graphics(currentDataGraphics);
}

void Opcode_00EE(uint16_t opcode, Chip8CPU &cpu)
{
    cpu.set_PC_Register(cpu.popStack());
}

void Opcode_00FD() // Use for Super Chip-8
{
    std::cout << "EXIT" << std::endl;
}

void Opcode_0nnn() // Don't use
{
    std::cout << "SYS addr" << std::endl;
}

void Opcode_8xy0(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;

    cpu.set_Registers(Vx, cpu.get_Registers(Vy));
}

void Opcode_8xy1(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;

    uint8_t OR = cpu.get_Registers(Vx) | cpu.get_Registers(Vy);
    cpu.set_Registers(Vx, OR);
}

void Opcode_8xy2(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;

    uint8_t AND = cpu.get_Registers(Vx) & cpu.get_Registers(Vy);
    cpu.set_Registers(Vx, AND);
}

void Opcode_8xy3(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;

    uint8_t XOR = cpu.get_Registers(Vx) ^ cpu.get_Registers(Vy);
    cpu.set_Registers(Vx, XOR);
}

void Opcode_8xy4(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;

    uint16_t ADD = cpu.get_Registers(Vx) + cpu.get_Registers(Vy);

    cpu.set_Registers(0x0F, ADD > 255 ? 1 : 0);
    cpu.set_Registers(Vx, ADD & 0xFF);
}

void Opcode_8xy5(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);
    uint8_t valueRegisterVy = cpu.get_Registers(Vy);

    cpu.set_Registers(0x0F, valueRegisterVx > valueRegisterVy ? 1 : 0);

    uint8_t SUB = (valueRegisterVx - valueRegisterVy) & 0xFF;
    cpu.set_Registers(Vx, SUB);
}

void Opcode_8xy6(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    cpu.set_Registers(0x0F, valueRegisterVx & 0x01);
    cpu.set_Registers(Vx, valueRegisterVx >> 1);
}

void Opcode_8xy7(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t Vy = (opcode >> 4) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);
    uint8_t valueRegisterVy = cpu.get_Registers(Vy);

    cpu.set_Registers(0x0F, valueRegisterVy > valueRegisterVx ? 1 : 0);

    uint8_t SUBN = (valueRegisterVy - valueRegisterVx) & 0xFF;
    cpu.set_Registers(Vx, SUBN);
}

void Opcode_8xyE(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    cpu.set_Registers(0x0F, (valueRegisterVx & 0x80) ? 1 : 0);
    cpu.set_Registers(Vx, valueRegisterVx << 1);
}

void Opcode_Ex9E(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    if(sharedKeyboard[valueRegisterVx] == 0x01)
    {
        cpu.set_PC_Register(cpu.get_PC_Register() + 2);

        {
            std::lock_guard<std::mutex> lock(keyboardMutex);
            sharedKeyboard[valueRegisterVx] = 0x00;
        }
    }

}

void Opcode_ExA1(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    if(sharedKeyboard[valueRegisterVx] != 0x01)
    {
        cpu.set_PC_Register(cpu.get_PC_Register() + 2);
    }
    else {
        {
            std::lock_guard<std::mutex> lock(keyboardMutex);
            sharedKeyboard[valueRegisterVx] = 0x00;
        }
    }
}

void Opcode_Fx07(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t DT = cpu.get_DT();

    cpu.set_Registers(Vx, DT);
}
void Opcode_Fx0A()
{
    std::cout << "LD Vx, K" << std::endl;
}
void Opcode_Fx15(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    cpu.set_DT(valueRegisterVx);
}

void Opcode_Fx18(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    cpu.set_ST(valueRegisterVx);
}

void Opcode_Fx1E(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint16_t ADD = cpu.get_I_Register() + cpu.get_Registers(Vx);

    cpu.set_I_Register(ADD);
}

void Opcode_Fx29(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t digit = cpu.get_Registers(Vx);

    uint16_t address = FRONT_BASE_ADDRESS + (digit * 5);
    cpu.set_I_Register(address);

}

void Opcode_Fx33(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint8_t valueRegisterVx = cpu.get_Registers(Vx);

    uint8_t hundreds = valueRegisterVx / 100;
    uint8_t tens     = (valueRegisterVx % 10) / 10;
    uint8_t ones     = valueRegisterVx % 10;

    uint16_t I_address = cpu.get_I_Register();
    systemMemory ROM = cpu.getter_ROM();
    ROM[I_address] = hundreds;
    ROM[I_address + 1] = tens;
    ROM[I_address + 2] = ones;
    
}


void Opcode_Fx55(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint16_t I_address = cpu.get_I_Register();
    systemMemory ROM = cpu.getter_ROM();

    for(uint8_t i = 0; i <= Vx; i++)
    {
        ROM[I_address + i] = cpu.get_Registers(i);
    }
}

void Opcode_Fx65(uint16_t opcode, Chip8CPU &cpu)
{
    uint8_t Vx = (opcode >> 8) & 0x0F;
    uint16_t I_address = cpu.get_I_Register();
    systemMemory ROM = cpu.getter_ROM();

    for(uint8_t i = 0; i <= Vx ; i++)
    {
        cpu.set_Registers(i, ROM[I_address + i]);
    }
}