# 定义编译器
CC = gcc

# 定义编译选项
CFLAGS = -Wall -g

# 定义源文件
SRCS = main.c ../llist/llist.c ../map/map.c ../Snake/Snake.c ../classic_mode/classic_mode.c ../Challenge_Mode/challenge_mode.c

# 定义可执行文件名
TARGET = Snake

# 目标规则
all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
