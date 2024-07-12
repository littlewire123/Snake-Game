#include "./classic_mode.h"

LLIST* init_classic_game(LLIST *snake_body_init)
{
    snake_body_init = classic_game_load();
    if(snake_body_init == NULL)
    {
        return NULL;
    }
    else
    {
        return snake_body_init;
    }
    return NULL;
}

void start_classic_game(LLIST *snake_body_start)
{
    snake_node tag;

    char input;

    STATIC_t.move_x = 0;
    STATIC_t.move_y = 1;
    STATIC_t.speed = 400000;

    srand(time(NULL));
    tag = get_tag();
    STATIC_t.tagx = tag.x;
    STATIC_t.tagy = tag.y;
    while (snake_find_body(snake_body_start,&tag , cmp_node) != NULL)
    {
        tag = get_tag();
        STATIC_t.tagx = tag.x;
        STATIC_t.tagy = tag.y;
    }

    STATIC_t.game_continue_flag = 1;
    printf("classic mode start\n");
    set_noblock_mode();

    while (STATIC_t.game_continue_flag)
    {
        
        system("clear");

        input = getchar();

        if(input == 'q' || input == 'Q')
        {
            classic_game_stroe(snake_body_start);
            end_classic_game(snake_body_start);
            return;
        }
        else if (input == 'w' || input == 'W')
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

        classic_move_snake(snake_body_start);
        print_classic(snake_body_start);

        usleep(STATIC_t.speed);
    }

    end_classic_game(snake_body_start);
    return ;
}

void continue_classic_game(LLIST *snake_body_continue)
{
    char input;

    srand(time(NULL));

    STATIC_t.game_continue_flag = 1;
    printf("classic mode continue\n");
    set_noblock_mode(); 
    
    
    while (STATIC_t.game_continue_flag)
    {
        
        system("clear");

        input = getchar();

        if(input == 'q' || input == 'Q')
        {
            classic_game_stroe(snake_body_continue);
            end_classic_game(snake_body_continue);
            return;
        }
        else if (input == 'w' || input == 'W')
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

        
        classic_move_snake(snake_body_continue);
        
        print_classic(snake_body_continue);

        usleep(STATIC_t.speed);
    }

    end_classic_game(snake_body_continue);
    return ;
}

void end_classic_game(LLIST *snake_body_end)
{
    reset_block_mode();
    STATIC_t.game_continue_flag = 0;
    printf("游戏结束，得分为%d\n", snake_body_end->count-LENGTH);
    return ;
}

void classic_move_snake(LLIST*snake_body_move)
{
    snake_node * save_node = NULL;

    NODE *head_node = snake_body_move->head.prev;
    snake_node *temp = (snake_node*)head_node->data;

    snake_node *check_node = NULL;
    check_node = (snake_node *)malloc(sizeof(snake_node));
    ERRP(NULL == check_node , chech_node NULL , goto ERR1);
    
    
    check_node->x = temp->x + STATIC_t.move_x;
    check_node->y = temp->y + STATIC_t.move_y;

     
    if (!check_point(snake_body_move,check_node))
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
        

        while(snake_find_body(snake_body_move,save_node , cmp_node) != NULL)
        {
            newtag = get_tag();
            STATIC_t.tagx = newtag.x;
            STATIC_t.tagy = newtag.y;

            save_node->x = newtag.x;
            save_node->y = newtag.y;
        }

        if(STATIC_t.speed != 200000)
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


void classic_game_stroe(LLIST *snake_body_stroe)
{
    FILE *fp = NULL;
    NODE *tail = NULL;
    fp = fopen(PATH_CLASSIC , "w");
    ERRP(NULL == fp , fp fopen , goto ERR1);

    ERRP(fwrite(&STATIC_t , sizeof(STATIC_t) , 1 , fp) != 1 , fwrite , goto ERR2);

    ERRP(fwrite(snake_body_stroe , sizeof(*snake_body_stroe) , 1, fp) != 1 , fwrite , goto ERR2);

    for(tail = snake_body_stroe->head.next ; tail != &snake_body_stroe->head ; tail = tail->next)
    {
        ERRP(fwrite(tail->data , snake_body_stroe->size , 1 , fp) != 1 , fwrite , goto ERR2);
    }

    fclose(fp);
    return ;
ERR2: 
    fclose(fp);
ERR1: 
    return ;
}

LLIST* classic_game_load()
{
    FILE *fp = NULL;
    LLIST *snake_body_load = NULL;
    void *tail = NULL;
    int i;
    fp = fopen(PATH_CLASSIC , "r");
    ERRP(NULL == fp ,fopen ,goto ERR1);

    getchar();
    ERRP(fread(&STATIC_t , sizeof(STATIC_t) , 1 , fp) != 1 , fread , goto ERR2);
    
    snake_body_load = (LLIST *)malloc(sizeof(LLIST));
    ERRP(NULL == snake_body_load , snake_body_load malloc , goto ERR2);

    ERRP(fread(snake_body_load , sizeof(*snake_body_load) , 1, fp )!= 1 , fread , goto ERR3);

    snake_body_load->head.data = NULL;
    snake_body_load->head.next = &snake_body_load->head;
    snake_body_load->head.prev = &snake_body_load->head;

    for(i=0 ; i<snake_body_load->count ;i++)
    {
        tail = (void *)malloc(snake_body_load->size);
        ERRP(NULL == tail , tail malloc , goto ERR3);

        ERRP(fread(tail, snake_body_load->size , 1 ,fp) != 1 , fread , goto ERR4);

        snake_insert_body(snake_body_load,tail);
        snake_body_load->count--;

        free(tail);
    }

    fclose(fp);
    return snake_body_load;
ERR4:
    free(tail);
ERR3: 
    free(snake_body_load);
ERR2: 
    fclose(fp);
ERR1: 
    return NULL;
}
