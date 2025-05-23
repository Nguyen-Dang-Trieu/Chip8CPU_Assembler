# Tên file thực thi
TARGET = chip8CPU

# Thư mục chứa mã nguồn và header
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# SFML include và lib
SFML_INC = -Iinclude
SFML_LIB = -Llib -lsfml-graphics -lsfml-window -lsfml-system

# Trình biên dịch và cờ biên dịch
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(INCLUDE_DIR) $(SFML_INC)
LDFLAGS = $(SFML_LIB)

# Các file mã nguồn
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Các file object
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Quy tắc mặc định
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET)

# Tạo thư mục build nếu chưa có
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Quy tắc biên dịch file object
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Quy tắc liên kết
$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Quy tắc clean
clean:
	rm -rf $(BUILD_DIR)
