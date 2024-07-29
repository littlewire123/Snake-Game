#include "./power_up_mode.h"

LLIST *init_power_game(LLIST *snake_body_power_init, LLIST **snake_obstacle_power, LLIST **snake_tag_power)
{
    snake_body_power_init = power_game_load(snake_obstacle_power, snake_tag_power);

    if (snake_body_power_init == NULL)
    {
        return NULL;
    }
    else
    {
        return snake_body_power_init;
    }
    return NULL;
}

void power_mode_start(LLIST *snake_body_start, LLIST *snake_obstacle_start, LLIST *snake_tag_start)
{
    set_noblock_mode(); // 函数用来设置输入不阻塞和禁用缓冲区
    STATIC_t.speed_false = 300000; // 辅助蛇获取真实的速度
    STATIC_t.speed_ture = 300000; // 蛇真实的速度
    lock_power = 1; // 蛇速度改变的一把锁

    srand(time(NULL));
    char input;
    STATIC_t.move_x = 0;
    STATIC_t.move_y = 1;

    STATIC_t.game_continue_flag = 1;
    STATIC_t.game_over_flag = 1;
    while (STATIC_t.game_continue_flag && STATIC_t.game_over_flag)
    {
        system("clear");

        input = getchar();

        if (input == 'q' || input == 'Q')
        {
            printf("\033[?25h");
            power_game_stroe(snake_body_start, snake_obstacle_start, snake_tag_start);
            timeout_power_game(snake_body_start);
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
        power_move_snake(snake_body_start, snake_obstacle_start, snake_tag_start);
        print_power(snake_body_start, snake_obstacle_start, snake_tag_start);
        usleep(STATIC_t.speed_ture);
    }
    if (STATIC_t.game_over_flag == 0)
    {
        printf("\033[?25h");
        power_mode_end(snake_body_start);
    }
    return;
}

void continue_power_game(LLIST *snake_body_start, LLIST *snake_obstacle_start, LLIST *snake_tag_start)
{
    set_noblock_mode(); // 函数用来设置输入不阻塞和禁用缓冲区

    char input;
    lock_power = 1;

    STATIC_t.game_continue_flag = 1;
    STATIC_t.game_over_flag = 1;

    while (STATIC_t.game_continue_flag && STATIC_t.game_over_flag)
    {
        system("clear");

        input = getchar();

        if (input == 'q' || input == 'Q')
        {
            printf("\033[?25h");
            power_game_stroe(snake_body_start, snake_obstacle_start, snake_tag_start);
            timeout_power_game(snake_body_start);
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
        power_move_snake(snake_body_start, snake_obstacle_start, snake_tag_start);
        print_power(snake_body_start, snake_obstacle_start, snake_tag_start);
        usleep(STATIC_t.speed_ture);
    }
    if (STATIC_t.game_over_flag == 0)
    {
        printf("\033[?25h");
        power_mode_end(snake_body_start);
    }
    return;
}

void power_mode_end(LLIST *snake_body_end)
{
    system("clear");
    reset_block_mode();
    FILE *fp = NULL;
    fp = fopen(PATH_POWER, "w");
    ERRP(NULL == fp, fopen, goto ERR1);

    printf("游戏结束，得分为%d\n", snake_body_end->count - LENGTH);

    fclose(fp);
    return;
ERR1:
    return;
}

void timeout_power_game(LLIST *snake_body_timeout)
{
    reset_block_mode();
    STATIC_t.game_continue_flag = 0;
    return;
}

void power_move_snake(LLIST *snake_body_move, LLIST *snake_obstacle_move, LLIST *snake_tag_move)
{
    if (power_mod_count != 0)
    {
        power_mod_count--;
    }
    else
    {
        lock_power = 1;
        STATIC_t.speed_ture = STATIC_t.speed_false;
    }
    
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

    if ((snake_find_tag(snake_tag_move, check_node, cmp_node)) != NULL)
    {
        snake_node newtag;

        snake_insert_body(snake_body_move, check_node);
        snake_del_tag(snake_tag_move, check_node);

        newtag = get_tag();

        while (snake_find_body(snake_body_move, &newtag, cmp_node) != NULL || snake_find_obstacle(snake_obstacle_move, &newtag, cmp_node) || snake_find_tag(snake_tag_move, &newtag, cmp_node) != NULL)
        {
            newtag = get_tag();
        }
        snake_insert_tag(snake_tag_move, &newtag);

        if (STATIC_t.speed_false != 150000 && lock_power)
        {
            STATIC_t.speed_false -= CHANGE_SPEED;
            STATIC_t.speed_ture = STATIC_t.speed_false;
        }
        else if (STATIC_t.speed_false == 150000 && lock_power)
        {
            STATIC_t.speed_ture = STATIC_t.speed_false;
        }
        free(check_node);

        snake_power_mod(snake_body_move, snake_obstacle_move, snake_tag_move, check_node);
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
    return;

ERR1:
    return;
}

//道具效果
void snake_power_mod(LLIST *snake_power_mod, LLIST *snake_obstacle_mod, LLIST *snake_tag_mod, snake_node *check_node)
{
    int num = 0;

    num = rand() % 100;

    if (num >= 0 && num < 10) // 加速
    {
        power_mod_count = 50;
        STATIC_t.speed_ture = 100000;
        lock_power = 0;
    }
    else if (num >= 10 && num < 20) // 减速
    {
        power_mod_count = 10;
        STATIC_t.speed_ture = 400000;
        lock_power = 0;
    }
    else if (num >= 20 && num <30) // 增加长度
    {
        if (snake_power_mod->count < 500)
        {
            snake_add_mod(snake_power_mod);
        }
        else
        {
            return;
        }
    }
    else if (num >= 30 && num < 40) // 减小长度
    {
        if (snake_power_mod->count > 3)
        {
            snake_del_mod(snake_power_mod);
        }
        else
        {
            return;
        }
    }
    else if (num >= 40 && num < 50) // 传送
    {
        snake_node newtag;
        newtag = get_tag();
        while (snake_find_body(snake_power_mod, &newtag, cmp_node) != NULL || snake_find_obstacle(snake_obstacle_mod, &newtag, cmp_node) || snake_find_tag(snake_tag_mod, &newtag, cmp_node) != NULL)
        {
            newtag = get_tag();
        }
        snake_insert_body(snake_power_mod, &newtag);
    }
    else
    {
        return;
    }
    return;
}

//道具：蛇身体缩减
void snake_del_mod(LLIST *snake_power_mod)
{
    NODE *tail = NULL;
    int num = 0;
    while (num < 3)
    {
        tail = snake_power_mod->head.prev;
        tail->prev->next = tail->next;
        tail->next->prev = tail->prev;
        free(tail->data);
        free(tail);
        snake_power_mod->count--;
        num++;
    }
    return;
}

//道具：蛇身体增加
void snake_add_mod(LLIST *snake_power_mod)
{
    NODE *tail = NULL;
    snake_node tag_mod;
    int num = 0;
    while (num < 3)
    {
        tail = snake_power_mod->head.prev;
        tag_mod.x = ((snake_node *)tail->data)->x + STATIC_t.move_x;
        tag_mod.y = ((snake_node *)tail->data)->y + STATIC_t.move_y;
        snake_insert_body(snake_power_mod , &tag_mod);
        num++;
    }
    return ;
}

void power_game_stroe(LLIST *snake_body_power, LLIST *snake_obstacle_power, LLIST *snake_tag_power)
{
    FILE *fp = NULL;
    NODE *tail = NULL;
    fp = fopen(PATH_POWER, "w");
    ERRP(NULL == fp, fopen, goto ERR1);

    //储存环境变量
    ERRP(fwrite(&STATIC_t, sizeof(STATIC_t), 1, fp) != 1, fwrite, goto ERR2);

    //储存障碍物信息
    ERRP(fwrite(snake_obstacle_power, sizeof(*snake_obstacle_power), 1, fp) != 1, fwrite, goto ERR2);

    for (tail = snake_obstacle_power->head.next; tail != &snake_obstacle_power->head; tail = tail->next)
    {
        ERRP(fwrite(tail->data, snake_obstacle_power->size, 1, fp) != 1, fwrite, goto ERR2);
    }

    //储存食物信息
    ERRP(fwrite(snake_tag_power, sizeof(*snake_tag_power), 1, fp) != 1, fwrite, goto ERR2);

    for (tail = snake_tag_power->head.next; tail != &snake_tag_power->head; tail = tail->next)
    {
        ERRP(fwrite(tail->data, snake_tag_power->size, 1, fp) != 1, fwrite, goto ERR2);
    }

    //储存蛇身体信息
    ERRP(fwrite(snake_body_power, sizeof(*snake_body_power), 1, fp) != 1, fwrite, goto ERR2);

    for (tail = snake_body_power->head.next; tail != &snake_body_power->head; tail = tail->next)
    {
        ERRP(fwrite(tail->data, snake_body_power->size, 1, fp) != 1, fwrite, goto ERR2);
    }
    fclose(fp);
    return;
ERR2:
    fclose(fp);
ERR1:
    return;
}

LLIST *power_game_load(LLIST **snake_obstacle_power, LLIST **snake_tag_power)
{
    FILE *fp = NULL;
    LLIST *snake_body_power_load = NULL;
    LLIST *snake_obstacle_temp = NULL;
    LLIST *snake_tag_temp = NULL;
    void *tail = NULL;
    int i;
    fp = fopen(PATH_POWER, "r");
    ERRP(NULL == fp, fopen, goto ERR1);

    //加载 环境变量
    ERRP(fread(&STATIC_t, sizeof(STATIC_t), 1, fp) != 1, fread, goto ERR2);

    //加载障碍物信息
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
    *snake_obstacle_power = snake_obstacle_temp;

    //加载食物信息
    snake_tag_temp = (LLIST *)malloc(sizeof(LLIST));
    ERRP(NULL == snake_tag_temp, snake_tag_temp malloc, goto ERR4);
    ERRP(fread(snake_tag_temp, sizeof(*snake_tag_temp), 1, fp) != 1, fread, goto ERR5);
    snake_tag_temp->head.data = NULL;
    snake_tag_temp->head.next = &snake_tag_temp->head;
    snake_tag_temp->head.prev = &snake_tag_temp->head;
    for (i = 0; i < snake_tag_temp->count; i++)
    {
        tail = (void *)malloc(sizeof(snake_tag_temp->size));
        ERRP(NULL == tail, tail malloc, goto ERR5);

        ERRP(fread(tail, snake_tag_temp->size, 1, fp) != 1, fread, goto ERR3);
        snake_insert_tag(snake_tag_temp, tail);

        snake_tag_temp->count--;
        free(tail);
    }
    *snake_tag_power = snake_tag_temp;

    //加载蛇身体信息
    snake_body_power_load = (LLIST *)malloc(sizeof(LLIST));
    ERRP(NULL == snake_body_power_load, snake_body_power_load malloc, goto ERR5);
    ERRP(fread(snake_body_power_load, sizeof(*snake_body_power_load), 1, fp) != 1, fread, goto ERR6);
    snake_body_power_load->head.data = NULL;
    snake_body_power_load->head.next = &snake_body_power_load->head;
    snake_body_power_load->head.prev = &snake_body_power_load->head;
    for (i = 0; i < snake_body_power_load->count; i++)
    {
        tail = (void *)malloc(sizeof(snake_body_power_load->size));
        ERRP(NULL == tail, tail malloc, goto ERR6);

        ERRP(fread(tail, snake_body_power_load->size, 1, fp) != 1, fread, goto ERR6);
        snake_insert_body(snake_body_power_load, tail);

        snake_body_power_load->count--;
        free(tail);
    }
    fclose(fp);
    return snake_body_power_load;
ERR6:
    if (tail != NULL)
    {
        free(tail);
    }
    free(snake_body_power_load);
ERR5:
    llist_destroy(&snake_tag_temp);
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
