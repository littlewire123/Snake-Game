#include "./classic_mode.h"

void start_classic_game()
{
    LLIST *snake_body = NULL;
    snake_node tag;
    snake_body = snake_init_body(LENGTH);

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
            end_classic_game(snake_body);
            return;
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

        classic_move_snake(snake_body, move_x, move_y);
        print_classic(snake_body);

        usleep(speed);
    }

    end_classic_game(snake_body);
    return ;
}

void end_classic_game(LLIST *snake_body)
{
    reset_block_mode();
    game_continue_flag = 0;
    printf("游戏结束，得分为%d\n", snake_body->count-LENGTH);
    return ;
}

void classic_move_snake(LLIST *snake_body, int move_x , int move_y)
{
    snake_node * save_node = NULL;

    NODE *head_node = snake_body->head.prev;
    snake_node *temp = (snake_node*)head_node->data;

    snake_node *check_node = NULL;
    check_node = (snake_node *)malloc(sizeof(snake_node));
    ERRP(NULL == check_node , chech_node NULL , goto ERR1);

    
    check_node->x = temp->x + move_x;
    check_node->y = temp->y + move_y;

    if (!check_point(snake_body, check_node))
    {
        game_continue_flag = 0;
        return;
    }


    if(check_node->x == tagx && check_node->y == tagy)
    {
        snake_node newtag;
        save_node = (snake_node *)malloc(sizeof(snake_node));
        ERRP(NULL == save_node , save_node malloc , goto ERR2);

        save_node->x = tagx;
        save_node->y = tagy;

        snake_insert_body(snake_body , save_node);
        newtag = get_tag();
        tagx = newtag.x;
        tagy = newtag.y;

        save_node->x = newtag.x;
        save_node->y = newtag.y;

        while(snake_find_body(snake_body , save_node , cmp_node) != NULL)
        {
            newtag = get_tag();
            tagx = newtag.x;
            tagy = newtag.y;

            save_node->x = newtag.x;
            save_node->y = newtag.y;
        }

        speed -= CHANGE_SPEED;
        free(check_node);
        free(save_node);
        return ;
    }

    NODE *tail = snake_body->head.next;
    while(tail->next != &snake_body->head)
    {
        memmove(tail->data , tail->next->data , snake_body->size);
        tail = tail->next;
    }
    memmove(tail->data , check_node , snake_body->size);

    free(check_node);
    free(save_node);
    return ;
ERR2: 
    free(check_node);
ERR1: 
    return ;
}
