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
            chioce_room_mod();
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

void chioce_room_mod()
{
    int highlight;
    int c;

    while (1)
    {
        system("clear");
        highlight = 0;

        while (1)
        {
            system("clear");
            print_roommenu(highlight);
            c = getch();

            if (c == 27)
            {
                getch();
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
                    if (highlight < 2)
                    {
                        ++highlight;
                    }
                }
            }
            else if (c == ENTER_KEY)
            {
                chioce_search_mod = highlight + 1;
                break;
            }
        }

        switch (chioce_search_mod)
        {
        case 1:
            system("clear");
            init_online_search();
            return;
        case 2:
            system("clear");
            chioce_online_mod();
            return;
        case 3:
            return;
        default:
            continue;
        }
    }
}

void chioce_online_mod()
{
    int highlight;
    int c;

    while (1)
    {
        system("clear");
        highlight = 0;

        while (1)
        {
            system("clear");
            print_online_menu(highlight);
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
                    if (highlight < 3)
                    {
                        ++highlight;
                    }
                }
            }
            else if (c == ENTER_KEY)
            {
                chioce_mod_online = highlight + 1;
                break;
            }
        }

        switch (chioce_mod_online)
        {
        case 1:
            system("clear");
            if (!init_online_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_game();
                printf("\033[?25h");
            }
            return;
        case 2:
            system("clear");
            if (!init_online_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_game();
                printf("\033[?25h");
            }
            return;
        case 3:
            system("clear");
            if (!init_online_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_game();
                printf("\033[?25h");
            }
            return;
        case 4:
            return;
        default:
            continue;
        }
    }
    return;
}

void submenu_select(int *mod) // 1.新游戏2.继续游戏3.退出
{
    int highlight = 0;
    int c;

    while (1)
    {
        system("clear");
        print_submenu(highlight);
        c = getch();

        if (c == 27)
        {
            getch(); //
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
                if (highlight < 2)
                {
                    ++highlight;
                }
            }
        }
        else if (c == ENTER_KEY)
        {
            *mod = highlight + 1;
            break;
        }
    }
}

void Classic_mod()
{
    int mod;
    if ((snake_body_classic = init_classic_game(snake_body_classic)) == NULL)
    {
        snake_body_classic = snake_init_body(snake_body_classic, LENGTH);
        start_classic_game(snake_body_classic);
    }
    else
    {
        system("clear");
        submenu_select(&mod);
        if (mod == 1)
        {
            snake_body_classic = snake_init_body(snake_body_classic, LENGTH);
            start_classic_game(snake_body_classic);
        }
        else if (mod == 2)
        {
            continue_classic_game(snake_body_classic);
        }
        else
        {
            system("clear");
            return;
        }
    }
    return;
}

void Challenge_mod()
{
    int mod;
    if ((snake_body_challenge = init_challenge_game(snake_body_challenge, &snake_obstacle_challenge)) == NULL)
    {
        snake_body_challenge = snake_init_body(snake_body_challenge, LENGTH);
        snake_obstacle_challenge = init_map(snake_body_challenge, snake_obstacle_challenge);
        challenge_mode_start(snake_body_challenge, snake_obstacle_challenge);
    }
    else
    {
        system("clear");
        submenu_select(&mod);
        if (mod == 1)
        {
            snake_body_challenge = snake_init_body(snake_body_challenge, LENGTH);
            snake_obstacle_challenge = init_map(snake_body_challenge, snake_obstacle_challenge);
            challenge_mode_start(snake_body_challenge, snake_obstacle_challenge);
        }
        else if (mod == 2)
        {
            continue_challenge_game(snake_body_challenge, snake_obstacle_challenge);
        }
        else
        {
            system("clear");
            return;
        }
    }
}

void Power_Up_mod()
{
    int mod;
    if ((snake_body_power = init_power_game(snake_body_power, &snake_obstacle_power, &snake_tag_power)) == NULL)
    {
        snake_body_power = snake_init_body(snake_body_power, LENGTH);
        snake_obstacle_power = init_map(snake_body_power, snake_obstacle_power);
        snake_tag_power = init_tag(snake_body_power, snake_tag_power, snake_obstacle_power);
        power_mode_start(snake_body_power, snake_obstacle_power, snake_tag_power);
    }
    else
    {
        system("clear");
        submenu_select(&mod);
        if (mod == 1)
        {
            snake_body_power = snake_init_body(snake_body_power, LENGTH);
            snake_obstacle_power = init_map(snake_body_power, snake_obstacle_power);
            snake_tag_power = init_tag(snake_body_power, snake_tag_power, snake_obstacle_power);
            power_mode_start(snake_body_power, snake_obstacle_power, snake_tag_power);
        }
        else if (mod == 2)
        {
            continue_power_game(snake_body_power, snake_obstacle_power, snake_tag_power);
        }
        else
        {
            system("clear");
            return;
        }
    }
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

void get_terminal(int *rows, int *cols)
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    *rows = ws.ws_row;
    *cols = ws.ws_col;
}

// 居中打印菜单
void print_centered_menu(const char **menu, int menu_size, int highlight)
{
    int rows, cols;
    get_terminal(&rows, &cols);

    int start_row = (rows - menu_size) / 2;
    int i;
    for (i = 0; i < menu_size; ++i)
    {
        int start_col = (cols - strlen(menu[i])) / 2;

        if (highlight == i)
        {
            HIGHLIGHT_TEXT();
            printf("\033[%d;%dH%s\n", start_row + i, start_col, menu[i]);
            RESET_TEXT();
        }
        else
        {
            YELLOW_TEXT();
            printf("\033[%d;%dH%s\n", start_row + i, start_col, menu[i]);
            RESET_TEXT();
        }
    }
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

void print_online_menu(int highlight)
{
    const char *menu[] = {
        "Classic_online_mode",
        "Challenge_online_mode",
        "Power_up_online_mode",
        "Exit"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu(menu, menu_size, highlight);
}

void print_submenu(int highlight)
{
    const char *menu[] = {
        "New game",
        "Continue game",
        "Exit"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu(menu, menu_size, highlight);
}

void print_roommenu(int highlight)
{
    const char *menu[] = {
        "Search for room",
        "Create a room",
        "Exit"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu(menu, menu_size, highlight);
}

void print_loginmenu(int highlight)
{
    const char *menu[] = {
        "input your id",
        "input your passwd",
        "login game",
        "sign up for a game"
    };
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu(menu , menu_size , highlight);
}

// 获取键盘输入
int getch()
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}