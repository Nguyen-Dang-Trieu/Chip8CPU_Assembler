#include "_utils.h"

void uint8_tToHex(uint8_t value)
{
    std::cout << "0x"
              << std::setw(2) << std::setfill('0') // Đảm bảo luôn có 2 chữ số
              << std::hex << std::uppercase        // In ra dạng HEX viết hoa
              << static_cast<int>(value)           // Tránh in ký tự lạ do uint8_t
              << std::endl;
}

void uint16_tToHex(uint16_t value)
{
    std::cout << "0x"
              << std::setw(4) << std::setfill('0') // 4 chữ số hex
              << std::hex << std::uppercase        // In dạng HEX viết hoa
              << value
              << std::endl;
}

void print_memory_ROM(const std::array<uint8_t, MEMORY_SIZE> &src, int count)
{
    for (int i = START_ADDRESS; i < count; i++)
    {
        uint8_tToHex(src[i]);
    }
}

// Dùng để hiển thị giá trị từng pixel của screen
void print_framebuffer(const std::array<std::array<uint8_t, 8>, 32>& framebuffer) {
    for (int y = 0; y < 32; ++y) {
        for (int x_byte = 0; x_byte < 8; ++x_byte) {
            uint8_t byte = framebuffer[y][x_byte];
            for (int bit = 7; bit >= 0; --bit) {
                bool pixel = (byte >> bit) & 0x01;
                std::cout << (pixel ? '1' : '0');
            }
            std::cout << ' '; // Cách mỗi byte 1 khoảng trắng
        }
        std::cout << '\n';
    }
}

void printStateKeyboard(const std::array<uint8_t, 16>& keyboard) {
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