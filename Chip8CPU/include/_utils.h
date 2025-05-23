#ifndef _UTILS_H
#define _UTILS_H

#include "chip8CPU.h"

void uint8_tToHex(uint8_t value);
void uint16_tToHex(uint16_t value);
void print_memory_ROM(const std::array<uint8_t, MEMORY_SIZE> &src, int count);
void print_framebuffer(const std::array<std::array<uint8_t, 8>, 32>& framebuffer);
void printStateKeyboard(const std::array<uint8_t, 16>& keyboard);

#endif // _UTILS_H