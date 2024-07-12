#include "./challenge_mode.h"

void ls(const void *data)
{
    printf("%d %d \n",((snake_node *)data)->x , ((snake_node *)data)->y);
    return;
}

void challenge_mode_start(LLIST *snake_body_start , LLIST *snake_obstacle_start)
{
    set_noblock_mode();// 函数用来设置输入不阻塞和禁用缓冲区

    snake_node tag;;
    char input;

    STATIC_t.move_x = 0;
    STATIC_t.move_y = 1;
    STATIC_t.speed = 300000;

    srand(time(NULL));

    tag = get_tag();
    STATIC_t.tagx = tag.x;
    STATIC_t.tagy = tag.y;

    while (snake_find_body(snake_body_start,&tag , cmp_node) != NULL || snake_find_obstacle(snake_obstacle_start,&tag ,cmp_node) != NULL)
    {
        tag = get_tag();
        STATIC_t.tagx = tag.x;
        STATIC_t.tagy = tag.y;
    }

    STATIC_t.game_continue_flag = 1;
    while (STATIC_t.game_continue_flag)
    {
        system("clear");

        input = getchar();

        if(input == 'q' || input == 'Q')
        {
            challene_mode_end(snake_body_start);
            return ;
        }

        if (input == 'w' || input == 'W')
        {
            STATIC_t.move_x = -1;
            STATIC_t.move_y = 0;
        }
        else if (input == 's' || input == 'S')
        {
            STATIC_t.move_x = 1;
            STATIC_t.move_y = 0;
        }
        else if (input == 'a' || input == 'A')
        {
            STATIC_t.move_x = 0;
            STATIC_t.move_y = -1;
        }
        else if (input == 'd' || input == 'D')
        {
            STATIC_t.move_x = 0;
            STATIC_t.move_y = 1;
        }
        challenge_move_snake(snake_body_start , snake_obstacle_start);
        print_challenge(snake_body_start,snake_obstacle_start);
        usleep(STATIC_t.speed);
    }
    challene_mode_end(snake_body_start);
}

void challene_mode_end(LLIST *snake_body_end)
{ 
    reset_block_mode();
    printf("游戏结束，得分为%d\n", snake_body_end->count-LENGTH);
    STATIC_t.game_continue_flag = 0;
    return ;
}

void challenge_move_snake(LLIST *snake_body_move , LLIST *snake_obstacle_move)
{
    snake_node * save_node = NULL;

    NODE *head_node = snake_body_move->head.prev;
    snake_node *temp = (snake_node*)head_node->data;

    snake_node *check_node = NULL;
    check_node = (snake_node *)malloc(sizeof(snake_node));
    ERRP(NULL == check_node , chech_node NULL , goto ERR1);

    
    check_node->x = temp->x + STATIC_t.move_x;
    check_node->y = temp->y + STATIC_t.move_y;

    if (!check_point(snake_body_move,check_node) || !check_obstacle(snake_obstacle_move, check_node))
    {
        STATIC_t.game_continue_flag = 0;
        return;
    }


    if(check_node->x == STATIC_t.tagx && check_node->y == STATIC_t.tagy)
    {
        snake_node newtag;
        save_node = (snake_node *)malloc(sizeof(snake_node));
        ERRP(NULL == save_node , save_node malloc , goto ERR2);

        save_node->x = STATIC_t.tagx;
        save_node->y = STATIC_t.tagy;

        snake_insert_body(snake_body_move,save_node);
        newtag = get_tag();
        STATIC_t.tagx = newtag.x;
        STATIC_t.tagy = newtag.y;

        save_node->x = newtag.x;
        save_node->y = newtag.y;

        while(snake_find_body(snake_body_move,save_node , cmp_node) != NULL || snake_find_obstacle(snake_obstacle_move,save_node , cmp_node))
        {
            newtag = get_tag();
            STATIC_t.tagx = newtag.x;
            STATIC_t.tagy = newtag.y;

            save_node->x = newtag.x;
            save_node->y = newtag.y;
        }

        if(STATIC_t.speed != 150000)
        {
            STATIC_t.speed -= CHANGE_SPEED;
        }
        free(check_node);
        free(save_node);
        return ;
    }

    NODE *tail = snake_body_move->head.next;
    while(tail->next != &snake_body_move->head)
    {
        memmove(tail->data , tail->next->data , snake_body_move->size);
        tail = tail->next;
    }
    memmove(tail->data , check_node , snake_body_move->size);

    free(check_node);
    free(save_node);
    return ;
ERR2: 
    free(check_node);
ERR1: 
    return ;
}