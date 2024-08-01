#include "./user.h"

void snake_destroy()
{
    if (snake_body_classic != NULL)
    {
        llist_destroy(&snake_body_classic);
    }
    if (snake_body_challenge != NULL)
    {
        llist_destroy(&snake_body_challenge);
    }
    if (snake_body_power != NULL)
    {
        llist_destroy(&snake_body_power);
    }
    if (snake_obstacle_challenge != NULL)
    {
        llist_destroy(&snake_obstacle_challenge);
    }
    if (snake_obstacle_power != NULL)
    {
        llist_destroy(&snake_obstacle_power);
    }
    if (snake_tag_power != NULL)
    {
        llist_destroy(&snake_tag_power);
    }
    destory_online();
}

void chioce_mod()
{
    int mod;
    int highlight;
    int c;

    srand(time(NULL));

    while (1)
    {
        system("clear");
        highlight = 0;

        while (1)
        {
            system("clear");
            print_menu(highlight);
            c = getch();

            if (c == 27)
            {
                getch(); // skip the [
                c = getch();

                if (c == UP_ARROW)
                {
                    if (highlight > 0)
                    {
                        --highlight;
                    }
                }
                else if (c == DOWN_ARROW)
                {
                    if (highlight < 5)
                    {
                        ++highlight;
                    }
                }
            }
            else if (c == ENTER_KEY)
            {
                mod = highlight + 1;
                break;
            }
        }

        switch (mod)
        {
        case 1:
            Classic_mod();
            break;
        case 2:
            Challenge_mod();
            break;
        case 3:
            Power_Up_mod();
            break;
        case 4:
            system("clear");
            // chioce_room_mod();
            login_online();
            break;
        case 5:
            About_Game();
            break;
        case 6:
            system("clear");
            return;
        default:
            break;
        }
        mod = 0;
    }
    return;
}

void About_Game()
{
    system("clear");

    printf("蛇的移动\n");
    printf("四方向移动（W:上 S:下 A:左 D:右）\n\n");
    printf("游戏中退出游戏并保存按Q\n\n");
    printf("1 ：经典模式：只有一个食物\n");
    printf("2 ：挑战模式 ：有一个食物和十个障碍物，速度会更快\n");
    printf("3 ：道具模式 ：有十个食物和十个障碍物，吃到食物有概率获取道具效果(加速、减速、增加3个长度、减小3个长度、随机传送)\n");
    printf("4 ：联机模式 ：经典模式下玩家对抗（其他模式联机模式开发中......）\n\n");
    printf("本项目遵循MIT开源协议；\n");
    printf("项目地址：https://github.com/littlewire123/Snake-Game\n\n");

    printf("按回车键退出...\n");

    while (getchar() != '\n')
    {
    }

    system("clear");
    return;
}


void print_menu(int highlight)
{
    const char *menu[] = {
        "Classic_mode",
        "Challenge_mode",
        "Power_up_mode",
        "Online_mode",
        "About_game",
        "Exit game"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu(menu, menu_size, highlight);
}
