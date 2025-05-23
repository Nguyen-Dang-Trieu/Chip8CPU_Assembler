#include <iostream>
#include <array>
#include <cstdint>

void printKeyboard(const std::array<uint8_t, 16>& keyboard) {
    std::cout << "   Keyboard State\n";
    for (int row = 0; row < 4; ++row) {
        std::cout << "  +-+-+-+-+\n  |";
        for (int col = 0; col < 4; ++col) {
            int index = row * 4 + col;
            std::cout << static_cast<int>(keyboard[index]) << "|";
        }
        std::cout << "\n";
    }
    std::cout << "  +-+-+-+-+\n";
}

int main()
{
    std::array<uint8_t, 16> sharedKeyboard = {};
    std::array<uint8_t, 16> test = {};
    sharedKeyboard[0x0] = 1; // Key '1'
    sharedKeyboard[0xA] = 1; // Key 'A'
    sharedKeyboard[0xF] = 1; // Key 'F'

    
    printKeyboard(sharedKeyboard);
    
    test = sharedKeyboard;
    test[0x00] = 0;
    printKeyboard(test);

    return 0;
}
