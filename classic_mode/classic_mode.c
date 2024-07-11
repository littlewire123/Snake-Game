#include "./classic_mode.h"

void start_classic_game()
{
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

    game_continue_flag = 1;
    printf("classic mode start\n");
    set_noblock_mode();

    while (game_continue_flag)
    {
        
        system("clear");

        input = getchar();

        if(input == 'q' || input == 'Q')
        {
            break;
        }
        else if (input == 'w' || input == 'W')
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

    end_classic_game(snake_body);
    return ;
}

void end_classic_game(LLIST *snake_body)
{
    reset_echo_mode();
    game_continue_flag = 0;
    printf("游戏结束，得分为%d\n", snake_body->count-lenth);
    return ;
}