#ifndef CHIP8CPU_H
#define CHIP8CPU_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <array>
#include <stack>    // std::stack
#include <vector>   // std::vector
#include <iomanip>
#include <map>
#include <functional>
#include <cstdint>  // uint8_t, ...
#include <thread>   // std::thread()
#include <chrono>
#include <atomic>
#include <mutex>    // std::mutex()
#include <cstdlib>  // rand(), srand()
#include <ctime>    // time()

#define MEMORY_SIZE 4096 // 4KB - bắt đàu lưu chương trình trình ROM tại vị trí [512]'
#define REGISTER_COUNT 16
#define STACK_SIZE 16
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define FONTSET_SIZE 80
#define FRONT_BASE_ADDRESS 80 // 0x050
#define START_ADDRESS 512 // 0x200


using frameBuffer = std::array<std::array<uint8_t, 8>, 32>; // [row x  column] = [32]x[8]
using systemMemory = std::array<uint8_t, MEMORY_SIZE>;

extern frameBuffer sharedScreenMemory;
extern bool updateBufferFlag;
extern std::mutex bufferMutex;
extern std::mutex keyboardMutex;
extern std::array<uint8_t, 16> sharedKeyboard;
extern std::atomic<uint8_t> delayTimer;

// Bộ Fontset standard (0-9, A-F)
const std::array<uint8_t, FONTSET_SIZE> Font_set = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

class Chip8CPU
{
private:
    // Memory 4KB
    systemMemory memory{};

    // Thanh ghi 8-bit (V0 - VF)
    std::array<uint8_t, REGISTER_COUNT> Registers{};

    // Thanh ghi địa chỉ 16-bit
    uint16_t I_Register = 0;

    // Program Counter
    uint16_t PC_Register = 0x200; // Chương trình bắt đầu từ địa chỉ 0x200

    // Stack & Stack Pointer
    std::stack<uint16_t> stack;

    // Timer
    uint8_t delay_timer = 0;
    uint8_t sound_timer = 0;

    // Màn hình 64x32 pixels (1-bit per pixel) -> row = 32 pixel, column = 64 pixel -> 8 byte pixel
    frameBuffer graphics{};

    // Trạng thái phím bấm (0x0 - 0xF)
    std::array<uint8_t, 16> keypad{};

public:
    bool drawFlag = false; // Được set là true khi DRW được thực thi
    std::map<uint8_t, std::function<void(uint16_t opcode, Chip8CPU &cpu)>> LookupTable_1;
    std::map<uint8_t, std::map<uint16_t, std::function<void(uint16_t opcode, Chip8CPU &cpu)>>> LookupTable_2; // Use for 0x00, 0x08, 0x0E, 0x0F

    Chip8CPU() { reset(); }
    size_t LoadROM(const char *fileName);
    void CPU_Run(std::array<uint8_t, MEMORY_SIZE> ROM);
    void initializeLookupTable();
    void timerThread();

    // Reset CPU
    void reset()
    {
        memory.fill(0);
        Registers.fill(0);
        stack = std::stack<uint16_t>();
        keypad.fill(0);

        I_Register = 0;
        PC_Register = 0x200;
        delay_timer = 0;
        sound_timer = 0;

        // Load Font vào bộ nhớ từ 0x50
        for (size_t i = 0; i < FONTSET_SIZE; i++)
        {
            memory[0x50 + i] = Font_set[i]; // memory[0x50] tương đương memory[80]
        }

        
        memory[0x00] = 0b10000000; 
        // set sprite cho program_main

        // Chữ I
        memory[0xA0] = 0b10000000;
        memory[0xA1] = 0b10000000;
        memory[0xA2] = 0b10000000;
        memory[0xA3] = 0b10000000;
        memory[0xA4] = 0b11110000;

        // chữ O
        memory[0xA5] = 0b11000000;
        memory[0xA6] = 0b11000000;

        // Chữ T
        memory[0xA7] = 0b01000000;
        memory[0xA8] = 0b11100000;

        memory[0xA9] = 0b10000000;
        memory[0xAA] = 0b11000000;
        memory[0xAB] = 0b10000000;

        memory[0xAC] = 0b01000000;
        memory[0xAD] = 0b11000000;
        memory[0xAE] = 0b01000000;

        memory[0xAF] = 0b11100000;
        memory[0xB0] = 0b01000000;

        // Chữ S
        memory[0xB1] = 0b01100000;
        memory[0xB2] = 0b11000000;

        memory[0xB3] = 0b10000000;
        memory[0xB4] = 0b11000000;
        memory[0xB5] = 0b01000000;

        // Chữ Z
        memory[0xB6] = 0b11000000;
        memory[0xB7] = 0b01100000;

        memory[0xB8] = 0b01000000;
        memory[0xB9] = 0b11000000;
        memory[0xBA] = 0b10000000;

        // Chữ J
        // Chữ L

    }

    // setter và getter
    systemMemory getter_ROM() const { return memory; }

    void set_I_Register(uint16_t value) { I_Register = value; }
    uint16_t get_I_Register() const { return I_Register; }

    void set_Registers(uint8_t index, uint8_t value) { Registers[index] = value; } // sett giá trị cho 16 register
    uint8_t get_Registers(uint8_t index) const { return Registers[index]; }

    void pushStack(uint16_t value) { stack.push(value); }
    uint16_t popStack() {
        if(!stack.empty()) {
            uint16_t val = stack.top();
            stack.pop();

            return val;
        }

        return 0; // or throw exception
    }
    size_t sizeStack() { return stack.size(); }

    void set_PC_Register(uint16_t value) { PC_Register = value; }
    uint16_t get_PC_Register() const { return PC_Register; }

    void set_Keypad(std::array<uint8_t, 16>& src) { keypad = src; }
    std::array<uint8_t, 16> get_Keypad() const { return keypad; }

    void set_graphics(frameBuffer src) { graphics = src; }
    frameBuffer get_graphics() const { return graphics; }

    // Timer
    void set_DT(uint8_t value) { 
        delay_timer = value;
        delayTimer.store(delay_timer);
    }
    uint8_t get_DT() const { return delayTimer.load(); }

    void set_ST(uint8_t value) { sound_timer = value; };
    uint8_t get_ST() const { return sound_timer; }


};

#endif