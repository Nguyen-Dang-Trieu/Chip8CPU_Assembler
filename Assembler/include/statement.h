#ifndef STATEMENT_H
#define STATEMENT_H

#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#8xy3
std::unordered_map<std::string, std::regex> Opcode_Table = {
    {"0nnn", std::regex(R"(SYS\s*(?:0x)?[0-9A-F]{3})")}, // SYS
    {"00E0", std::regex(R"(CLS)")},                      // CLS
    {"00EE", std::regex(R"(RET)")},                      // RET
    {"1nnn", std::regex(R"(JP\s+((?:0x)?[0-9A-F]{3}|\w+))")},
    {"2nnn", std::regex(R"(CALL\s*(?:0x)?[0-9A-F]{3})")},                       // CALL
    {"3xkk", std::regex(R"(SE\s*V[0-9A-F],\s*(?:0x)?[0-9A-F]{2})")},            // SE
    {"4xkk", std::regex(R"(SNE\s*V[0-9A-F],\s*(?:0x)?[0-9A-F]{2})")},           // SNE
    {"5xy0", std::regex(R"(SE\s*V[0-9A-F],\s*V[0-9A-F])")},                     // SE Vx, Vy
    {"6xkk", std::regex(R"(LD\s*V[0-9A-F],\s*(?:0x)?[0-9A-F]{2})")},            // LD Vx, byte
    {"7xkk", std::regex(R"(ADD\s*V[0-9A-F],\s*(?:0x)?[0-9A-F]{2})")},           // ADD Vx, byte
    {"8xy0", std::regex(R"(LD\s*V[0-9A-F],\s*V[0-9A-F])")},                     // LD Vx, Vy
    {"8xy1", std::regex(R"(OR\s*V[0-9A-F],\s*V[0-9A-F])")},                     // OR Vx, Vy
    {"8xy2", std::regex(R"(AND\s*V[0-9A-F],\s*V[0-9A-F])")},                    // AND Vx, Vy
    {"8xy3", std::regex(R"(XOR\s*V[0-9A-F],\s*V[0-9A-F])")},                    // XOR Vx, Vy
    {"8xy4", std::regex(R"(ADD\s*V[0-9A-F],\s*V[0-9A-F])")},                    // ADD Vx, Vy
    {"8xy5", std::regex(R"(SUB\s*V[0-9A-F],\s*V[0-9A-F])")},                    // SUB Vx, Vy
    {"8xy6", std::regex(R"(SHR\s*V[0-9A-F](?:\s*,\s*V[0-9A-F])?)")},            // SHR Vx, Vy
    {"8xy7", std::regex(R"(SUBN\s*V[0-9A-F],\s*V[0-9A-F])")},                   // SUBN Vx, Vy
    {"8xyE", std::regex(R"(SHL\s*V[0-9A-F](?:\s*,\s*V[0-9A-F])?)")},            // SHL Vx, Vy
    {"9xy0", std::regex(R"(SNE\s*V[0-9A-F],\s*V[0-9A-F])")},                    // SNE Vx, Vy
    {"Annn", std::regex(R"(LD\s*I,\s*(?:0x)?[0-9A-F]{3})")},                    // LD I, addr
    {"Bnnn", std::regex(R"(JP\s*V0,\s*(?:0x)?[0-9A-F]{3})")},                   // JP V0, addr
    {"Cxkk", std::regex(R"(RND\s*V[0-9A-F],\s*(?:0x)?[0-9A-F]{2})")},           // RND
    {"Dxyn", std::regex(R"(DRW\s*V[0-9A-F],\s*V[0-9A-F],\s*(?:0x)?[0-9A-F])")}, // DRW Vx, Vy, nibble
    {"Ex9E", std::regex(R"(SKP\s*V[0-9A-F])")},                                 // SKP Vx
    {"ExA1", std::regex(R"(SKNP\s*V[0-9A-F])")},                                // SKNP
    {"Fx07", std::regex(R"(LD\s*V[0-9A-F],\s*DT)")},                            // LD Vx, DT
    {"Fx0A", std::regex(R"(LD\s*V[0-9A-F],\s*K)")},                             // LD Vx, K
    {"Fx15", std::regex(R"(LD\s*DT,\s*V[0-9A-F])")},                            // LD DT, Vx
    {"Fx18", std::regex(R"(LD\s*ST,\s*V[0-9A-F])")},                            // LD ST, Vx
    {"Fx1E", std::regex(R"(ADD\s*I,\s*V[0-9A-F])")},                            // ADD I Vx
    {"Fx29", std::regex(R"(LD\s*F,\s*V[0-9A-F])")},                             // LD F, Vx
    {"Fx33", std::regex(R"(LD\s*B,\s*V[0-9A-F])")},                             // LD B, Vx
    {"Fx55", std::regex(R"(LD\s*\[I\],\s*V[0-9A-F])")},                         // LD [I], Vx
    {"Fx65", std::regex(R"(LD\s*V[0-9A-F],\s*\[I\])")}                          // LD Vx, [I]
    
};

std::string detectOpcode(const std::string& instruction){
    for(const auto &[opcode, pattern] : Opcode_Table){
        if(std::regex_match(instruction, pattern))
        {
            return opcode;
        }
    }
    return "Unknown";
}

#endif