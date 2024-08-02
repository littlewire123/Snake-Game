# 定义编译器
CC = gcc

# 定义编译选项
CFLAGS = -Wall -g

# 定义源文件
SRCS = main.c ./user.c ../llist/llist.c ./online_menu.c ./single_menu.c ../map/map.c ../Snake/Snake.c ../Classic_Mode/classic_mode.c ../Challenge_Mode/challenge_mode.c ../Power_up_Mode/power_up_mode.c ../Online_Mode/Online_General/online_mode.c ../Online_Mode/Protocol/protocol.c  -lpthread

# 定义可执行文件名
TARGET = snake

# 目标规则
all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
