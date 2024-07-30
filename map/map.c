#include "./map.h"

int cmp_node(const void *d1, const void *d2)
{
    snake_node *nd1 = (snake_node *)d1;
    snake_node *nd2 = (snake_node *)d2;
    if (nd1->x == nd2->x && nd1->y == nd2->y)
    {
        return 0;
    }
    return 1;
}

void set_noblock_mode()
{
    struct termios term;
    int flag;

    if (tcgetattr(STDIN_FILENO, &term) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    // 关闭缓冲区
    term.c_lflag &= ~(ICANON | ECHO);

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // 设置非阻塞
    flag = fcntl(0, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(0, F_SETFL, flag);
}

void reset_block_mode()
{
    struct termios term;
    int flag;

    // 恢复终端设置
    if (tcgetattr(STDIN_FILENO, &term) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    term.c_lflag |= (ICANON | ECHO); // 重新开启规范模式和回显
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // 恢复标准输入的阻塞模式
    flag = fcntl(STDIN_FILENO, F_GETFL);
    if (flag == -1)
    {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }
    flag &= ~O_NONBLOCK; // 清除非阻塞标志
    if (fcntl(STDIN_FILENO, F_SETFL, flag) == -1)
    {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

snake_node get_tag()
{
    snake_node node;
    node.x = (rand() % (M - 2)) + 1;
    node.y = (rand() % (N - 2)) + 1;
    return node;
}

int check_point(LLIST *snake_body_check, snake_node *node)
{
    if (snake_find_body(snake_body_check, node, cmp_node) != NULL || node->x == 0 || node->x == M - 1 || node->y == 0 || node->y == N - 1)
    {
        return 0;
    }
    return 1;
}

int check_obstacle(LLIST *snake_body_check, snake_node *node)
{
    if (snake_find_obstacle(snake_body_check, node, cmp_node) != NULL || node->x == 0 || node->x == M - 1 || node->y == 0 || node->y == N - 1)
    {
        return 0;
    }
    return 1;
}

int check_tag(LLIST *snake_tag_chech, snake_node *node)
{
    if (snake_find_tag(snake_tag_chech, node, cmp_node) != NULL || node->x == 0 || node->x == M - 1 || node->y == 0 || node->y == N - 1)
    {
        return 0;
    }
    return 1;
}

void get_terminal_size(int *rows, int *cols)
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    *rows = ws.ws_row;
    *cols = ws.ws_col;
}

void print_classic(LLIST *snake_body_find)
{
    // 保存光标位置
    printf("\0337");

    // 隐藏光标
    printf("\033[?25l");

    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);

    int offset_x = (term_cols - N) / 2;
    int offset_y = (term_rows - M) / 2;

    int i, j;
    for (i = 0; i < offset_y; ++i)
    {
        printf("\n"); // 在顶部打印适当数量的空行，以实现垂直居中
    }

    for (i = 0; i < M; ++i)
    {
        printf("%*s", offset_x, ""); // 在每行前打印适当数量的空格，以实现水平居中

        for (j = 0; j < N; ++j)
        {
            snake_node find_body;
            find_body.x = i;
            find_body.y = j;

            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
            {
                BLUE_TEXT();
                printf("#");
                RESET_TEXT();
            }
            else if (snake_find_body(snake_body_find, &find_body, cmp_node) != NULL)
            {
                GREEN_TEXT();
                printf("⚫");
                RESET_TEXT();
            }
            else if (i == STATIC_t.tagx && j == STATIC_t.tagy)
            {
                YELLOW_TEXT();
                printf("❤");
                RESET_TEXT();
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("%*s", offset_x, ""); // 在得分行前打印适当数量的空格，以实现水平居中
    YELLOW_TEXT();
    printf("得分 : %d\n", snake_body_find->count - LENGTH);
    printf("%*s", offset_x, ""); // 在控制提示行前打印适当数量的空格，以实现水平居中
    printf("向上：W，向下：S，向左：A，向右：D 退出并保存：Q\n");
    RESET_TEXT();

    // 恢复光标位置
    printf("\0338");

    // 显示光标
    printf("\033[?25h");

    fflush(stdout);

    return;
}

void print_challenge(LLIST *snake_body_find, LLIST *snake_obstacle_find)
{
    // 隐藏光标
    printf("\033[?25l");

    // 保存光标位置
    printf("\0337");

    // 清屏
    system("clear");

    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);

    int offset_x = (term_cols - N) / 2;
    int offset_y = (term_rows - M) / 2;

    int i, j;
    for (i = 0; i < offset_y; ++i)
    {
        printf("\n"); // 在顶部打印适当数量的空行，以实现垂直居中
    }

    for (i = 0; i < M; ++i)
    {
        printf("%*s", offset_x, ""); // 在每行前打印适当数量的空格，以实现水平居中

        for (j = 0; j < N; ++j)
        {
            snake_node find_body, find_obstacle;
            find_body.x = i;
            find_body.y = j;

            find_obstacle.x = i;
            find_obstacle.y = j;

            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
            {
                BLUE_TEXT();
                printf("#");
                RESET_TEXT();
            }
            else if (snake_find_body(snake_body_find, &find_body, cmp_node) != NULL)
            {
                GREEN_TEXT();
                printf("⚫");
                RESET_TEXT();
            }
            else if (snake_find_obstacle(snake_obstacle_find, &find_obstacle, cmp_node) != NULL)
            {
                MAGENTA_TEXT();
                printf("☢");
                RESET_TEXT();
            }
            else if (i == STATIC_t.tagx && j == STATIC_t.tagy)
            {
                RED_TEXT();
                printf("❤");
                RESET_TEXT();
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("%*s", offset_x, ""); // 在得分行前打印适当数量的空格，以实现水平居中
    YELLOW_TEXT();
    printf("得分 : %d\n", snake_body_find->count - LENGTH);
    printf("%*s", offset_x, ""); // 在控制提示行前打印适当数量的空格，以实现水平居中
    printf("向上：W，向下：S，向左：A，向右：D 退出并保存：Q\n");
    RESET_TEXT();

    // 恢复光标位置
    printf("\0338");

    // 显示光标
    printf("\033[?25h");

    fflush(stdout);
    return;
}

void print_power(LLIST *snake_body_find, LLIST *snake_obstacle_find, LLIST *snake_tag_power_find)
{
    // 隐藏光标
    printf("\033[?25l");

    // 保存光标位置
    printf("\0337");

    int term_rows, term_cols;
    get_terminal_size(&term_rows, &term_cols);

    int offset_x = (term_cols - N) / 2;
    int offset_y = (term_rows - M) / 2;

    int i, j;
    for (i = 0; i < offset_y; ++i)
    {
        printf("\n"); // 在顶部打印适当数量的空行，以实现垂直居中
    }

    for (i = 0; i < M; ++i)
    {
        printf("%*s", offset_x, ""); // 在每行前打印适当数量的空格，以实现水平居中

        for (j = 0; j < N; ++j)
        {
            snake_node find_body, find_obstacle, find_tag;
            find_body.x = i;
            find_body.y = j;

            find_obstacle.x = i;
            find_obstacle.y = j;

            find_tag.x = i;
            find_tag.y = j;

            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
            {
                printf("\033[34m#\033[0m"); // 边界
            }
            else if (snake_find_body(snake_body_find, &find_body, cmp_node) != NULL)
            {
                printf("\033[32m⚫\033[0m"); // 蛇身
            }
            else if (snake_find_obstacle(snake_obstacle_find, &find_obstacle, cmp_node) != NULL)
            {
                printf("\033[35m☢\033[0m"); // 障碍物
            }
            else if (snake_find_tag(snake_tag_power_find, &find_tag, cmp_node) != NULL)
            {
                printf("\033[31m❤\033[0m"); // 能量标签
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }

    printf("%*s", offset_x, ""); // 在得分行前打印适当数量的空格，以实现水平居中
    printf("\033[33m得分 : %d\n", snake_body_find->count - LENGTH);

    printf("%*s", offset_x, ""); // 在控制提示行前打印适当数量的空格，以实现水平居中
    printf("向上：W，向下：S，向左：A，向右：D 退出并保存：Q\033[0m\n");

    // 恢复光标位置
    printf("\0338");

    // 显示光标
    printf("\033[?25h");

    fflush(stdout);
    return;
}


LLIST *init_map(LLIST *snake_body_find, LLIST *snake_obstacle_init)
{
    obstacle_num = 10;
    int num = 0;

    snake_obstacle_init = llist_creat(sizeof(snake_node));
    ERRP(NULL == snake_obstacle_init, snake_obstacle_init malloc, goto ERR1);

    snake_node obstacle;

    while (num < 10)
    {
        obstacle = get_tag();
        if (!check_obstacle(snake_obstacle_init, &obstacle) || !check_point(snake_body_find, &obstacle))
        {
            obstacle = get_tag();
        }
        snake_insert_obstacle(snake_obstacle_init, &obstacle);
        num++;
    }
    return snake_obstacle_init;

ERR1:
    return NULL;
}

void snake_insert_obstacle(LLIST *snake_obstacle_insert, snake_node *obstacle)
{
    llist_append(snake_obstacle_insert, obstacle);
    return;
}

LLIST *init_tag(LLIST *snake_body_check, LLIST *snake_tag_init, LLIST *snake_obstacle_check)
{
    tag_num = 10;
    int num = 0;

    snake_tag_init = llist_creat(sizeof(snake_node));
    ERRP(NULL == snake_tag_init, snake_tag_init malloc, goto ERR1);

    snake_node tag;

    while (num < 10)
    {
        tag = get_tag();
        if (!check_obstacle(snake_obstacle_check, &tag) || !check_tag(snake_tag_init, &tag) || !check_point(snake_body_check, &tag))
        {
            tag = get_tag();
        }
        snake_insert_tag(snake_tag_init, &tag);
        num++;
    }
    return snake_tag_init;
ERR1:
    return NULL;
}

void snake_insert_tag(LLIST *snake_tag_insert, snake_node *tag)
{
    llist_append(snake_tag_insert, tag);
    return;
}
