#include <SFML/Graphics.hpp>
#include "chip8CPU.h"
#include "_utils.h"

std::array<uint8_t, 16> sharedKeyboard;

/* **************** CPU THREAD ****************** */
void cpuThread()
{
    std::cout << "Thread CPU" << std::endl;

    Chip8CPU cpu;
    size_t byte_ROM = cpu.LoadROM("D:\\WORK_SPACE\\Chip8_Emulator_start\\output.ch8");
    std::array<uint8_t, MEMORY_SIZE> ROM = cpu.getter_ROM();
    cpu.CPU_Run(ROM);

    uint8_tToHex(cpu.get_Registers(0x00));
    uint8_tToHex(cpu.get_Registers(0x01));

    // doạn code dùng để test trạng thái nút nhấn
    while (true)
    {
        if (sharedKeyboard[0] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[0] = 0x00;
        }
        else if (sharedKeyboard[1] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[1] = 0x00;
        }
        else if (sharedKeyboard[2] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[2] = 0x00;
        }
        else if (sharedKeyboard[3] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[3] = 0x00;
        }
        else if (sharedKeyboard[4] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[4] = 0x00;
        }
        else if (sharedKeyboard[5] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[5] = 0x00;
        }
        else if (sharedKeyboard[6] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[6] = 0x00;
        }
        else if (sharedKeyboard[7] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[7] = 0x00;
        }
        else if (sharedKeyboard[8] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[8] = 0x00;
        }
        else if (sharedKeyboard[9] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[9] = 0x00;
        }
        else if (sharedKeyboard[10] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[10] = 0x00;
        }
        else if (sharedKeyboard[11] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[11] = 0x00;
        }
        else if (sharedKeyboard[12] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[12] = 0x00;
        }
        else if (sharedKeyboard[13] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[13] = 0x00;
        }
        else if (sharedKeyboard[14] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[14] = 0x00;
        }
        else if (sharedKeyboard[15] == 0x01)
        {
            printStateKeyboard(sharedKeyboard);
            sharedKeyboard[15] = 0x00;
        }
    }
}

/* **************** SFML THREAD ****************** */
void sfmlThread()
{
    std::cout << "Thread SFML" << std::endl;

    // ------------------------------------------ INITIALIZE ----------------------------------------------
    const int PIXEL_SIZE = 15;

    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH * PIXEL_SIZE, SCREEN_HEIGHT * PIXEL_SIZE}), "Chip 8");
    sf::Image image({64, 32}, sf::Color::Black); // width x height = column x row
    sf::Texture texture;

    // ------------------------------------------ INITIALIZE ----------------------------------------------

    while (window.isOpen())
    {
        // ---------------------------------------- EVENT ------------------------------------------------
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();

            // Key board
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[0] = 0x01;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[1] = 0x01;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[2] = 0x01;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[3] = 0x01;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[4] = 0x01;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[5] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[6] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[7] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[8] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[9] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[10] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[11] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[12] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[13] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[14] = 0x01;
                }
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
            {
                {
                    std::lock_guard<std::mutex> lock(keyboardMutex);
                    sharedKeyboard[15] = 0x01;
                }
            }
        }
        // ---------------------------------------- EVENT ------------------------------------------------

        // ---------------------------------------- UPDATE ------------------------------------------------
        {
            std::lock_guard<std::mutex> lock(bufferMutex);
            if (updateBufferFlag)
            {
                for (int y = 0; y < SCREEN_HEIGHT; ++y)
                {
                    for (int xByte = 0; xByte < SCREEN_WIDTH / 8; ++xByte)
                    {
                        uint8_t byte = sharedScreenMemory[y][xByte];

                        // Duyệt qua từng bit trong byte
                        for (int bit = 0; bit < 8; ++bit)
                        {
                            if ((byte >> (7 - bit)) & 1)
                            {
                                // Nếu bit là 1, vẽ pixel màu trắng
                                image.setPixel(sf::Vector2u(xByte * 8 + bit, y), sf::Color::Green);
                            }
                            else
                            {
                                // Nếu bit là 0, vẽ pixel màu đen
                                image.setPixel(sf::Vector2u(xByte * 8 + bit, y), sf::Color::Black);
                            }
                        }
                    }
                }
                updateBufferFlag = false;
            }
        }
        if (!texture.loadFromImage(image))
        {
            std::cerr << "Error: Could not load texture from image!" << std::endl;
        }
        sf::Sprite sprite(texture);
        sprite.setScale(sf::Vector2f(15.0f, 15.0f));
        // ---------------------------------------- UPDATE ------------------------------------------------

        // ---------------------------------------- DISPLAY ------------------------------------------------
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
        // ---------------------------------------- DISPLAY ------------------------------------------------
    }
}

int main()
{
    std::thread cpu(cpuThread);
    std::thread sfml(sfmlThread);

    cpu.join();
    sfml.join();

    return 0;
}
