#include "./challenge_mode.h"


void challenge_mode_start()
{
    set_noblock_mode();// 函数用来设置输入不阻塞和禁用缓冲区

    LLIST *snake_body = NULL;
    snake_node tag;
    snake_body = snake_init_body(3);

    char input;

    int move_x = 0;
    int move_y = 1;
    speed = 500000;

    srand(time(NULL));
    tag = get_tag();
    tagx = tag.x;
    tagy = tag.y;
    while (snake_find_body(snake_body, &tag , cmp_node) != NULL)
    {
        tag = get_tag();
        tagx = tag.x;
        tagy = tag.y;
    }


    while (1)
    {
        printf("test1\n");
        system("clear");

        input = getchar();

        if(input == 'q' || input == 'Q')
        {
            challene_mode_end(snake_body);
            return ;
        }

        if (input == 'w' || input == 'W')
        {
            move_x = -1;
            move_y = 0;
        }
        else if (input == 's' || input == 'S')
        {
            move_x = 1;
            move_y = 0;
        }
        else if (input == 'a' || input == 'A')
        {
            move_x = 0;
            move_y = -1;
        }
        else if (input == 'd' || input == 'D')
        {
            move_x = 0;
            move_y = 1;
        }

        move_snake(snake_body, move_x, move_y);
        print(snake_body);
        usleep(speed);
    }

}

void challene_mode_end(LLIST *snake_body)
{
    printf("游戏结束，得分为%d\n", snake_body->count-lenth);
    reset_echo_mode();
    return ;
}