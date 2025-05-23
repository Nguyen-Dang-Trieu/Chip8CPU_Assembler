#include "chip8CPU.h"
#include "OpcodeHanlers.h"

frameBuffer sharedScreenMemory;
bool updateBufferFlag = false;
std::mutex bufferMutex;
std::mutex keyboardMutex;
std::atomic<uint8_t> delayTimer = 0;

void Chip8CPU::initializeLookupTable()
{   
    // ----------------------------------- ** Lookup Table 1 ** ----------------------------------- //
    LookupTable_1[0x01] = Opcode_1nnn;
    LookupTable_1[0x02] = Opcode_2nnn;
    LookupTable_1[0x03] = Opcode_3xkk;
    LookupTable_1[0x04] = Opcode_4xkk;
    LookupTable_1[0x05] = Opcode_5xy0;
    LookupTable_1[0x06] = Opcode_6xkk;
    LookupTable_1[0x07] = Opcode_7xkk;
    LookupTable_1[0x09] = Opcode_9xy0;
    LookupTable_1[0x0A] = Opcode_Annn;
    LookupTable_1[0x0B] = Opcode_Bnnn;
    LookupTable_1[0x0C] = Opcode_Cxkk; 
    LookupTable_1[0x0D] = Opcode_Dxyn;

    // ----------------------------------- ** Lookup Table 2 ** ----------------------------------- //
    LookupTable_2[0x00][0xE0] = Opcode_00E0;
    LookupTable_2[0x00][0xEE] = Opcode_00EE;

    LookupTable_2[0x08][0x00] = Opcode_8xy0;
    LookupTable_2[0x08][0x01] = Opcode_8xy1;
    LookupTable_2[0x08][0x02] = Opcode_8xy2;
    LookupTable_2[0x08][0x03] = Opcode_8xy3;
    LookupTable_2[0x08][0x04] = Opcode_8xy4;
    LookupTable_2[0x08][0x05] = Opcode_8xy5;
    LookupTable_2[0x08][0x06] = Opcode_8xy6;
    LookupTable_2[0x08][0x07] = Opcode_8xy7;
    LookupTable_2[0x08][0x0E] = Opcode_8xyE;
    
    LookupTable_2[0x0E][0x9E] = Opcode_Ex9E;
    LookupTable_2[0x0E][0xA1] = Opcode_ExA1;

    LookupTable_2[0x0F][0x07] = Opcode_Fx07;
    LookupTable_2[0x0F][0x15] = Opcode_Fx15;
    LookupTable_2[0x0F][0x18] = Opcode_Fx18;
    LookupTable_2[0x0F][0x1E] = Opcode_Fx1E;
    LookupTable_2[0x0F][0x29] = Opcode_Fx29;
    LookupTable_2[0x0F][0x33] = Opcode_Fx33;
    LookupTable_2[0x0F][0x55] = Opcode_Fx55;
    LookupTable_2[0x0F][0x65] = Opcode_Fx65;
    
}

void Chip8CPU::CPU_Run(std::array<uint8_t, MEMORY_SIZE> ROM)
{
    bool EXIT = true;
    initializeLookupTable(); // Khởi tạo bảng tra cứu opcode

    // Timer
    std::thread Timer(&Chip8CPU::timerThread, this);
    Timer.detach();

    while (EXIT)
    {
        // ------------------------ Fetch ------------------------ 
        uint8_t byte1 = ROM.at(PC_Register); PC_Register += 1;
        uint8_t byte2 = ROM.at(PC_Register); PC_Register += 1;
        uint16_t opcode = 0x0000;
        opcode = (byte1 << 8) | byte2;

        if ((opcode & 0x00FF) == 0x00FD)
        {
            std::cout << "End!" << std::endl;
            EXIT = false;
            break;
        }
        
        // ------------------------ Decode and Execute ------------------------
        uint8_t highByte_highNibble_opcode = (opcode >> 12) & 0x0F;        // lấy 4 bit đầu của opcode
        uint8_t lowByte_opcode = opcode & 0xFF;                            // lấy 8 bit cuối của opcode
        uint8_t lowByte_lowNibble_opcode = opcode & 0x0F;                  // lấy 4 bit cuối của opcode

        switch (highByte_highNibble_opcode)
        {
            case 0x00:
            {
                switch (lowByte_opcode)
                {
                    case 0xE0: LookupTable_2[0x00][0xE0](opcode, *this); break;
                    case 0xEE: LookupTable_2[0x00][0xEE](opcode, *this); break;
                    default: Opcode_0nnn(); break;
                }
                break;
            }
            case 0x01: LookupTable_1[0x01](opcode, *this); break;
            case 0x02: LookupTable_1[0x02](opcode, *this); break;
            case 0x03: LookupTable_1[0x03](opcode, *this); break;
            case 0x04: LookupTable_1[0x04](opcode, *this); break;
            case 0x05: LookupTable_1[0x05](opcode, *this); break;
            case 0x06: LookupTable_1[0x06](opcode, *this); break;
            case 0x07: LookupTable_1[0x07](opcode, *this); break;
            case 0x08:
            {
                switch (lowByte_lowNibble_opcode)
                {
                    case 0x00: LookupTable_2[0x08][0x00](opcode, *this); break;
                    case 0x01: LookupTable_2[0x08][0x01](opcode, *this); break;
                    case 0x02: LookupTable_2[0x08][0x02](opcode, *this); break;
                    case 0x03: LookupTable_2[0x08][0x03](opcode, *this); break;
                    case 0x04: LookupTable_2[0x08][0x04](opcode, *this); break;
                    case 0x05: LookupTable_2[0x08][0x05](opcode, *this); break;
                    case 0x06: LookupTable_2[0x08][0x06](opcode, *this); break;
                    case 0x07: LookupTable_2[0x08][0x07](opcode, *this); break;
                    case 0x0E: LookupTable_2[0x08][0x0E](opcode, *this); break;
                    default: break;
                }
                break;
            }
            case 0x09: LookupTable_1[0x09](opcode, *this); break;
            case 0x0A: LookupTable_1[0x0A](opcode, *this); break;
            case 0x0B: LookupTable_1[0x0B](opcode, *this); break;
            case 0x0C: LookupTable_1[0x0C](opcode, *this); break;
            case 0x0D: 
            {
                LookupTable_1[0x0D](opcode, *this);  // Dữ liệu đã update trong graphics
                if(drawFlag)
                {
                    {
                        std::lock_guard<std::mutex> lock(bufferMutex);
                        sharedScreenMemory = get_graphics();
                        updateBufferFlag = true;
                    }
                    drawFlag = false;
                }
                break;
            }
            case 0x0E: 
            {
                switch (lowByte_opcode)
                {
                    case 0x9E: LookupTable_2[0x0E][0x9E](opcode, *this); break;
                    case 0xA1: LookupTable_2[0x0E][0xA1](opcode, *this); break;
                    default: break;
                }
                break;
            }

            case 0x0F: 
            {
                switch (lowByte_opcode)
                {
                    case 0x07: LookupTable_2[0x0F][0x07](opcode, *this); break;
                    //case 0x0A: LookupTable_2[0x0F][0x0A]; break;
                    case 0x15: LookupTable_2[0x0F][0x15](opcode, *this); break;
                    case 0x18: LookupTable_2[0x0F][0x18](opcode, *this); break;
                    case 0x1E: LookupTable_2[0x0F][0x1E](opcode, *this); break;
                    case 0x29: LookupTable_2[0x0F][0x29](opcode, *this); break;
                    case 0x33: LookupTable_2[0x0F][0x33](opcode, *this); break;
                    case 0x55: LookupTable_2[0x0F][0x55](opcode, *this); break;
                    case 0x65: LookupTable_2[0x0F][0x65](opcode, *this); break;
                    default: break;
                }
                break;
            }
            default: break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1200)); // ~833Hz
    }
}

// Tải dữ liệu vào ROM và trả về số byte thực tế đã được lưu
size_t Chip8CPU::LoadROM(const char *fileName)
{
    std::ifstream file(fileName, std::ios::binary | std::ios::in | std::ios::ate);
    std::vector<char> memblock; // lưu dữ liệu tạm thời đọc được từ file
    std::streampos fileSize;

    if (file.is_open())
    {
        fileSize = file.tellg(); // lấy kích thước của file
        if (fileSize == 0)
        {
            file.close();
            return 0;
        }

        memblock.assign(fileSize, 0);
        file.seekg(0, std::ios::beg);
        file.read(memblock.data(), fileSize);
        file.close();
    }
    else
    {
        std::cerr << "Can't open this file!";
    }

    // Load vào trong ROM, bắt đầu tại địa chỉ 0x200
    std::memcpy(memory.data() + 512, memblock.data(), fileSize);

    return static_cast<size_t>(fileSize);
}

void Chip8CPU::timerThread(){
    std::cout << "Timer Thread" << std::endl;

    while(true){
        // The timer frequency is 60Hz = 16.66667 ms
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); 

        uint8_t currentTimerValue = delayTimer.load();

        // Check current Timer value
        // std::cout << "Delay Timer: " << std::dec << (int)currentTimerValue << std::endl;

        if(currentTimerValue > 0)
            delayTimer.store(currentTimerValue - 1);
        
    }
}