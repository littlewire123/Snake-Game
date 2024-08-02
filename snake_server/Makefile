# Makefile

# 定义编译器和编译选项
CXX := g++

# 定义目标文件和源文件
TARGET := snake_server
SOURCES := main.cpp
LIB := -lpthread -lcrypt

# 默认目标
all: $(TARGET)

# 编译目标
$(TARGET):$(SOURCES)
	$(CXX) $^ $(LIB) -o $@

# 清理目标
clean:
	rm -f $(TARGET)

# 依赖目标
.PHONY: all clean
