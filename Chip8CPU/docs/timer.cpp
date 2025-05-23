#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

// Biến atomic để lưu trữ giá trị của Delay Timer (an toàn cho đa luồng)
std::atomic<uint8_t> delayTimerValue = 0;

// Hàm để thiết lập giá trị của Delay Timer từ luồng chính của emulator
void setDelayTimer(uint8_t value) {
    delayTimerValue.store(value);
}

// Hàm để lấy giá trị hiện tại của Delay Timer từ luồng chính của emulator
uint8_t getDelayTimer() {
    return delayTimerValue.load();
}

// Hàm chạy trong luồng riêng để cập nhật Delay Timer
void delayTimerThread() {
    while (true) {
        // Chờ khoảng 1/60 giây
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Khoảng 16.67 ms

        // Giảm giá trị của Delay Timer nếu nó lớn hơn 0
        uint8_t currentTimerValue = delayTimerValue.load();
        if (currentTimerValue > 0) {
            delayTimerValue.store(currentTimerValue - 1);
        }

        std::cout << "Child thread: Run" << std::endl;
    }
}

int main() {
    // Khởi chạy luồng Delay Timer
    std::thread dtThread(delayTimerThread);
    dtThread.detach(); // Cho phép luồng chạy độc lập và kết thúc khi chương trình chính kết thúc

    // --- Phần mô phỏng vòng lặp chính của emulator ---
    for (int i = 0; i < 300; ++i) {
        // Mô phỏng việc thiết lập Delay Timer (ví dụ, khi gặp lệnh Fx15)
        if (i == 50) {
            std::cout << "main thread: set gia tri ban dau cua delay = 50" << std::endl;
            setDelayTimer(50); // 120 frame = 2 giây (120 / 60)
        }

        if(i == 100) {
            std::cout << "Main thread: set lai gia tri delay timer = 40" << std::endl;
            setDelayTimer(40);
        }
        // Lấy và in giá trị của Delay Timer
        std::cout << "Main thread: Gia tri delay timer hien tai la: " << static_cast<int>(getDelayTimer()) << std::endl;

        // Mô phỏng các hoạt động khác của emulator (ví dụ, xử lý opcode)
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        if(i == 299){
            std::cout << "gia tri cua i:" << i << std::endl;
            std::cout << "Main thread: End!" << std::endl;
        }
    }

    

    // Lưu ý: Vì luồng dtThread đã được detach(), chúng ta không cần join() nó.
    // Nó sẽ tự động kết thúc khi chương trình chính kết thúc.

    return 0;
}