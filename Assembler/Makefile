# Tên file thực thi
TARGET = AssemblerChip8

# Thư mục chứa mã nguồn và header
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

# Trình biên dịch và cờ biên dịch
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -I$(INCLUDE_DIR)

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
	$(CXX) $(OBJECTS) -o $@

# Quy tắc clean xóa các file object và file thực thi và thư mục build
clean:
	rm -rf $(BUILD_DIR)
