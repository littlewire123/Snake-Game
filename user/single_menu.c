#include "single_menu.h"


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
                if (highlight < 3)
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

void print_submenu(int highlight)
{
    const char *menu[] = {
        "New game",
        "Continue game",
        "Exit"};
    int menu_size = sizeof(menu) / sizeof(menu[0]);
    print_centered_menu(menu, menu_size, highlight);
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
