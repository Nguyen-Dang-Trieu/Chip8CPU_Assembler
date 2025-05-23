#ifndef OPCODE_HANDLERS_H
#define OPCODE_HANDLERS_H

#include "chip8CPU.h"

// Dành cho Lookup Table 1
void Opcode_1nnn(uint16_t opcode, Chip8CPU &cpu); // JP addr
void Opcode_2nnn(uint16_t opcode, Chip8CPU &cpu); // CALL addr
void Opcode_3xkk(uint16_t opcode, Chip8CPU &cpu); // SE Vx, byte
void Opcode_4xkk(uint16_t opcode, Chip8CPU &cpu); // SNE Vx, byte
void Opcode_5xy0(uint16_t opcode, Chip8CPU &cpu); // SE Vx, Vy
void Opcode_6xkk(uint16_t opcode, Chip8CPU &cpu); // LD Vx, byte
void Opcode_7xkk(uint16_t opcode, Chip8CPU &cpu); // ADD Vx, byte
void Opcode_9xy0(uint16_t opcode, Chip8CPU &cpu); // SNE Vx, Vy
void Opcode_Annn(uint16_t opcode, Chip8CPU &cpu); // LD I, addr
void Opcode_Bnnn(uint16_t opcode, Chip8CPU &cpu); // Bnnn - JP V0, addr
void Opcode_Cxkk(uint16_t opcode, Chip8CPU &cpu); // Cxkk - RND Vx, byte
void Opcode_Dxyn(uint16_t opcode, Chip8CPU &cpu); // DRW Vx, Vy, nibble

// Dành cho lookup Table 2
void Opcode_00E0(uint16_t opcode, Chip8CPU &cpu); // CLS
void Opcode_00EE(uint16_t opcode, Chip8CPU &cpu); // RET
void Opcode_00FD();                               // For Super Chip-8
void Opcode_0nnn();                               // Don't use

void Opcode_8xy0(uint16_t opcode, Chip8CPU &cpu); // LD Vx, Vy
void Opcode_8xy1(uint16_t opcode, Chip8CPU &cpu); // OR Vx, Vy
void Opcode_8xy2(uint16_t opcode, Chip8CPU &cpu); // AND Vx, Vy
void Opcode_8xy3(uint16_t opcode, Chip8CPU &cpu); // XOR Vx, Vy
void Opcode_8xy4(uint16_t opcode, Chip8CPU &cpu); // ADD Vx, Vy
void Opcode_8xy5(uint16_t opcode, Chip8CPU &cpu); // SUB Vx, Vy
void Opcode_8xy6(uint16_t opcode, Chip8CPU &cpu); // SHR Vx {, Vy}
void Opcode_8xy7(uint16_t opcode, Chip8CPU &cpu); // SUBN Vx, Vy
void Opcode_8xyE(uint16_t opcode, Chip8CPU &cpu); // SHL Vx {, Vy

void Opcode_Ex9E(uint16_t opcode, Chip8CPU &cpu); // SKP Vx
void Opcode_ExA1(uint16_t opcode, Chip8CPU &cpu); // SKNP Vx

void Opcode_Fx07(uint16_t opcode, Chip8CPU &cpu); // LD Vx, DT
void Opcode_Fx0A();
void Opcode_Fx15(uint16_t opcode, Chip8CPU &cpu); // LD DT, Vx
void Opcode_Fx18(uint16_t opcode, Chip8CPU &cpu); // LD ST, Vx
void Opcode_Fx1E(uint16_t opcode, Chip8CPU &cpu); // ADD I, Vx
void Opcode_Fx29(uint16_t opcode, Chip8CPU &cpu); // LD F, Vx
void Opcode_Fx33(uint16_t opcode, Chip8CPU &cpu); // LD B, Vx
void Opcode_Fx55(uint16_t opcode, Chip8CPU &cpu); // LD [I], Vx
void Opcode_Fx65(uint16_t opcode, Chip8CPU &cpu); // LD Vx, [I]

#endif // OPCODE_HANDLERS_H