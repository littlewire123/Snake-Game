#ifndef __STATIC_H__
#define __STATIC_H__

#define UP_ARROW 65
#define DOWN_ARROW 66
#define ENTER_KEY 10

#define M 26 //地图高度
#define N 60 //地图宽度
#define CHANGE_SPEED 5000 //吃到食物增加的速度
#define LENGTH 3 //初始长度
#define PATH_CLASSIC "../Game_db/classic_game.db" //储存经典模式路径
#define PATH_CHALLENGE "../Game_db/challenge_game.db" //储存挑战模式路径
#define PATH_POWER "../Game_db/power_up_game.db" //储存道具模式路径
#define CLEAR_SCREEN() printf("\033[H\033[J") // 清屏
#define RED_TEXT() printf("\033[31m")        // 红色文字
#define GREEN_TEXT() printf("\033[32m")      // 绿色文字
#define YELLOW_TEXT() printf("\033[33m")     // 黄色文字
#define BLUE_TEXT() printf("\033[34m")       // 蓝色文字
#define RESET_TEXT() printf("\033[0m")       // 重置文字颜色
#define CYAN_TEXT() printf("\033[36m")       // 青色文字
#define MAGENTA_TEXT() printf("\033[35m")    // 紫色文字
#define HIGHLIGHT_TEXT() printf("\033[1;7m") //在终端输出带有高亮效果的文本

#include "../llist/llist.h"
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

typedef struct static_t{
    int lenth;
    int speed_ture;
    int speed_false;
    int tagx;
    int tagy;
    int game_continue_flag;
    int game_over_flag;
    int move_x;
    int move_y;
}STATIC;
STATIC STATIC_t;

int obstacle_num; // 障碍物的数量
int tag_num; //食物的数量
int power_mod_count; // 道具模式中，蛇获取效果后走的步数
int lock_power; // 道具模式中,蛇速度变化的一把锁

int chioce_search_mod; //联网道具模式的选择选项1.搜索房间2.创建房间
int chioce_mod_online; //联网模式的模式选择

LLIST * snake_obstacle_challenge; //挑战模式障碍物
LLIST * snake_obstacle_power; //道具模式障碍物

LLIST * snake_body_classic; //经典模式身体
LLIST * snake_body_challenge;//挑战模式身体
LLIST * snake_body_power;//道具模式身体

LLIST * snake_tag_power;//道具模式食物

#endif