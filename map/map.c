#include "./map.h"

void print_snake(snake_node *node)
{
    printf("x: %d , y: %d",node->x,node->y);
    putchar(10);
}

int cmp_node(const void *d1 , const void *d2)
{
    snake_node * nd1 = (snake_node *)d1;
    snake_node * nd2 = (snake_node *)d2;
    if(nd1->x == nd2->x && nd1->y == nd2->y)
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

    //设置非阻塞
    flag = fcntl(0, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(0, F_SETFL, flag);
}

void reset_block_mode() {
    struct termios term;
    int flag;

    // 恢复终端设置
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }
    term.c_lflag |= (ICANON | ECHO); // 重新开启规范模式和回显
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }

    // 恢复标准输入的阻塞模式
    flag = fcntl(STDIN_FILENO, F_GETFL);
    if (flag == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }
    flag &= ~O_NONBLOCK; // 清除非阻塞标志
    if (fcntl(STDIN_FILENO, F_SETFL, flag) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

snake_node get_tag()
{
    snake_node node;
    node.x = (rand() % (M-2)) + 1;
    node.y = (rand() % (N-2)) + 1;
    return node;
}


int check_point(LLIST* snake_body_check, snake_node *node)
{
    if (snake_find_body(snake_body_check, node , cmp_node) != NULL || node->x == 0 || node->x == M-1 || node->y ==0 || node->y == N-1)
    {
        return 0;
    }
    return 1;
}

int check_obstacle(LLIST* snake_body_check,snake_node *node)
{
    if (snake_find_obstacle(snake_body_check,node , cmp_node) != NULL || node->x == 0 || node->x == M-1 || node->y ==0 || node->y == N-1)
    {
        return 0;
    }
    return 1;
}

void print_classic(LLIST *snake_body_find)
{
    
    int i, j;
    for (i=0; i<M; ++i)
    {
        for (j=0; j<N; ++j)
        {
            snake_node find_body;
            find_body.x = i;
            find_body.y = j;

            if (i==0 || j==0 || i==M-1 || j==N-1)
            {
                printf("#");
            }
            else if (snake_find_body(snake_body_find,&find_body , cmp_node) != NULL)
            {
                printf("O");
            }
            else if (i==STATIC_t.tagx && j==STATIC_t.tagy)
            {
                printf("x");
            }
            else
            {
                printf(" ");
            }

        }
        printf("\n");
    }

    printf("向上：W，向下：S，向左：A，向右：D\n");
}

void print_challenge(LLIST *snake_body_find , LLIST *snake_obstacle_find)
{
    int i, j;
    for (i=0; i<M; ++i)
    {
        for (j=0; j<N; ++j)
        {
            snake_node find_body , find_obstacle;
            find_body.x = i;
            find_body.y = j;

            find_obstacle.x = i;
            find_obstacle.y = j;

            if (i==0 || j==0 || i==M-1 || j==N-1)
            {
                printf("#");
            }
            else if (snake_find_body(snake_body_find,&find_body , cmp_node) != NULL)
            {
                printf("O");
            }
            else if(snake_find_obstacle(snake_obstacle_find,&find_obstacle , cmp_node) != NULL)
            {
                printf("@");
            }
            else if (i==STATIC_t.tagx && j==STATIC_t.tagy)
            {
                printf("x");
            }
            else
            {
                printf(" ");
            }

        }
        printf("\n");
    }

    printf("向上：W，向下：S，向左：A，向右：D\n");
}



LLIST *init_map(LLIST * snake_obstacle_init)
{
    obstacle_num = 10;
    int num = 0;
    
    snake_obstacle_init = llist_creat(sizeof(snake_node));
    ERRP(NULL == snake_obstacle_init , snake_obstacle_init malloc , goto ERR1);

    snake_node obstacle;

    while(num < 10)
    {
        obstacle = get_tag();
        if(!check_obstacle(snake_obstacle_init,&obstacle))
        {
            obstacle = get_tag();
        }
        snake_insert_obstacle(snake_obstacle_init , &obstacle);
        num++;
    }
    return snake_obstacle_init;

ERR1: 
    return NULL;
}

void snake_insert_obstacle(LLIST* snake_obstacle_insert,  snake_node *obstacle)
{
    llist_append(snake_obstacle_insert, obstacle);
    return ;
}

