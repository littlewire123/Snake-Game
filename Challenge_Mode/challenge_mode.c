#include "./challenge_mode.h"

LLIST *init_challenge_game(LLIST *snake_body_init, LLIST **snake_obstacle)
{
    snake_body_init = challenge_game_load(snake_obstacle);
    if (snake_body_init == NULL)
    {
        return NULL;
    }
    else
    {
        return snake_body_init;
    }
    return NULL;
}

void challenge_mode_start(LLIST *snake_body_start, LLIST *snake_obstacle_start)
{
    set_noblock_mode(); // 函数用来设置输入不阻塞和禁用缓冲区

    snake_node tag;
    char input;

    STATIC_t.move_x = 0;
    STATIC_t.move_y = 1;
    STATIC_t.speed = 300000;

    srand(time(NULL));

    tag = get_tag();
    STATIC_t.tagx = tag.x;
    STATIC_t.tagy = tag.y;

    while (snake_find_body(snake_body_start, &tag, cmp_node) != NULL || snake_find_obstacle(snake_obstacle_start, &tag, cmp_node) != NULL)
    {
        tag = get_tag();
        STATIC_t.tagx = tag.x;
        STATIC_t.tagy = tag.y;
    }

    STATIC_t.game_continue_flag = 1;
    STATIC_t.game_over_flag = 1;

    while (STATIC_t.game_continue_flag && STATIC_t.game_over_flag)
    {
        system("clear");

        input = getchar();

        if (input == 'q' || input == 'Q')
        {
            challenge_game_stroe(snake_body_start , snake_obstacle_start);
            timeout_challenge_game(snake_body_start);
            return;
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
        challenge_move_snake(snake_body_start, snake_obstacle_start);
        print_challenge(snake_body_start, snake_obstacle_start);
        usleep(STATIC_t.speed);
    }
    if(STATIC_t.game_over_flag == 0)
    {
        challene_mode_end(snake_body_start);
    }
    return ;
}

void continue_challenge_game(LLIST *snake_body_start, LLIST *snake_obstacle_start)
{
    set_noblock_mode(); // 函数用来设置输入不阻塞和禁用缓冲区

    char input;
    srand(time(NULL));

    STATIC_t.game_continue_flag = 1;
    STATIC_t.game_over_flag = 1;

    while (STATIC_t.game_continue_flag && STATIC_t.game_over_flag)
    {
        system("clear");

        input = getchar();

        if (input == 'q' || input == 'Q')
        {
            challenge_game_stroe(snake_body_start , snake_obstacle_start);
            timeout_challenge_game(snake_body_start);
            return;
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
        challenge_move_snake(snake_body_start, snake_obstacle_start);
        print_challenge(snake_body_start, snake_obstacle_start);
        usleep(STATIC_t.speed);
    }
    if(STATIC_t.game_over_flag == 0)
    {
        challene_mode_end(snake_body_start);
    }
    return ;
}

void challene_mode_end(LLIST *snake_body_end)
{
    reset_block_mode();
    FILE *fp = NULL;
    fp = fopen(PATH_CHALLENGE , "w");
    ERRP(NULL == fp , fopen , goto ERR1);

    printf("游戏结束，得分为%d\n", snake_body_end->count - LENGTH);

    fclose(fp);
    return;
ERR1: 
    return ;
}

void timeout_challenge_game(LLIST *snake_body_timeout)
{
    reset_block_mode();
    STATIC_t.game_continue_flag = 0;
    return ;
}

void challenge_move_snake(LLIST *snake_body_move, LLIST *snake_obstacle_move)
{
    snake_node *save_node = NULL;

    NODE *head_node = snake_body_move->head.prev;
    snake_node *temp = (snake_node *)head_node->data;

    snake_node *check_node = NULL;
    check_node = (snake_node *)malloc(sizeof(snake_node));
    ERRP(NULL == check_node, chech_node NULL, goto ERR1);

    check_node->x = temp->x + STATIC_t.move_x;
    check_node->y = temp->y + STATIC_t.move_y;

    if (!check_point(snake_body_move, check_node) || !check_obstacle(snake_obstacle_move, check_node))
    {
        STATIC_t.game_over_flag = 0;
        return;
    }

    if (check_node->x == STATIC_t.tagx && check_node->y == STATIC_t.tagy)
    {
        snake_node newtag;
        save_node = (snake_node *)malloc(sizeof(snake_node));
        ERRP(NULL == save_node, save_node malloc, goto ERR2);

        save_node->x = STATIC_t.tagx;
        save_node->y = STATIC_t.tagy;

        snake_insert_body(snake_body_move, save_node);
        newtag = get_tag();
        STATIC_t.tagx = newtag.x;
        STATIC_t.tagy = newtag.y;

        save_node->x = newtag.x;
        save_node->y = newtag.y;

        while (snake_find_body(snake_body_move, save_node, cmp_node) != NULL || snake_find_obstacle(snake_obstacle_move, save_node, cmp_node))
        {
            newtag = get_tag();
            STATIC_t.tagx = newtag.x;
            STATIC_t.tagy = newtag.y;

            save_node->x = newtag.x;
            save_node->y = newtag.y;
        }

        if (STATIC_t.speed != 150000)
        {
            STATIC_t.speed -= CHANGE_SPEED;
        }
        free(check_node);
        free(save_node);
        return;
    }

    NODE *tail = snake_body_move->head.next;
    while (tail->next != &snake_body_move->head)
    {
        memmove(tail->data, tail->next->data, snake_body_move->size);
        tail = tail->next;
    }
    memmove(tail->data, check_node, snake_body_move->size);

    free(check_node);
    free(save_node);
    return;
ERR2:
    free(check_node);
ERR1:
    return;
}

void challenge_game_stroe(LLIST *snake_body_challenge, LLIST *snake_stroe_obstacle)
{
    FILE *fp = NULL;
    NODE *tail = NULL;
    fp = fopen(PATH_CHALLENGE, "w");
    ERRP(NULL == fp, fopen, goto ERR1);

    ERRP(fwrite(&STATIC_t, sizeof(STATIC_t), 1, fp) != 1, fwrite, goto ERR2);

    ERRP(fwrite(snake_stroe_obstacle, sizeof(*snake_stroe_obstacle), 1, fp) != 1, fwrite, goto ERR2);

    for (tail = snake_stroe_obstacle->head.next; tail != &snake_stroe_obstacle->head; tail = tail->next)
    {
        ERRP(fwrite(tail->data, snake_stroe_obstacle->size, 1, fp) != 1, fwrite, goto ERR2);
    }

    ERRP(fwrite(snake_body_challenge, sizeof(*snake_body_challenge), 1, fp) != 1, fwrite, goto ERR2);

    for (tail = snake_body_challenge->head.next; tail != &snake_body_challenge->head; tail = tail->next)
    {
        ERRP(fwrite(tail->data, snake_body_challenge->size, 1, fp) != 1, fwrite, goto ERR2);
    }

    fclose(fp);
    return;
ERR2:
    fclose(fp);
ERR1:
    return;
}

LLIST *challenge_game_load(LLIST **snake_obstacle)
{
    FILE *fp = NULL;
    LLIST *snake_body_challenge_load = NULL;
    LLIST *snake_obstacle_temp = NULL;
    void *tail = NULL;
    int i;
    fp = fopen(PATH_CHALLENGE, "r");
    ERRP(NULL == fp, fopen, goto ERR1);

    ERRP(fread(&STATIC_t, sizeof(STATIC_t), 1, fp) != 1, fread, goto ERR2);

    snake_obstacle_temp = (LLIST *)malloc(sizeof(LLIST));
    ERRP(snake_obstacle_temp == NULL, snake_obstacle_temp malloc, goto ERR2);

    ERRP(fread(snake_obstacle_temp, sizeof(*snake_obstacle_temp), 1, fp) != 1, fread, goto ERR3);

    snake_obstacle_temp->head.data = NULL;
    snake_obstacle_temp->head.next = &snake_obstacle_temp->head;
    snake_obstacle_temp->head.prev = &snake_obstacle_temp->head;

    for (i = 0; i < snake_obstacle_temp->count; i++)
    {
        tail = (void *)malloc(sizeof(snake_obstacle_temp->size));
        ERRP(NULL == tail, tail malloc, goto ERR3);

        ERRP(fread(tail, snake_obstacle_temp->size, 1, fp) != 1, fread, goto ERR3);
        snake_insert_obstacle(snake_obstacle_temp, tail);

        snake_obstacle_temp->count--;
        free(tail);
    }
    *snake_obstacle = snake_obstacle_temp;

    snake_body_challenge_load = (LLIST *)malloc(sizeof(LLIST));
    ERRP(NULL == snake_body_challenge_load, snake_body_challenge_load malloc, goto ERR4);

    ERRP(fread(snake_body_challenge_load, sizeof(*snake_body_challenge_load), 1, fp) != 1, fread, goto ERR5);

    snake_body_challenge_load->head.data = NULL;
    snake_body_challenge_load->head.next = &snake_body_challenge_load->head;
    snake_body_challenge_load->head.prev = &snake_body_challenge_load->head;

    for (i = 0; i < snake_body_challenge_load->count; i++)
    {
        tail = (void *)malloc(snake_body_challenge_load->size);
        ERRP(tail == NULL, tail malloc, goto ERR6);

        ERRP(fread(tail , snake_body_challenge_load->size , 1, fp) != 1 , fread , goto ERR6);
        snake_insert_body(snake_body_challenge_load , tail);

        snake_body_challenge_load->count--;
        free(tail);
    }

    fclose(fp);
    return snake_body_challenge_load;

ERR6:
    if (tail != NULL)
    {
        free(tail);
    }
ERR5:
    free(snake_body_challenge_load);
ERR4:
    llist_destroy(&snake_obstacle_temp);
    goto ERR2;
ERR3:
    if (tail != NULL)
    {
        free(tail);
    }
    free(snake_obstacle_temp);
ERR2:
    fclose(fp);
ERR1:
    return NULL;
}