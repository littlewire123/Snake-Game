#include "./map.h"
#include "../user/static.h"

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

void move_snake(LLIST *snake_body, int move_x , int move_y)
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
        //printf("游戏结束，得分为%d\n", snake_body->count-lenth);
        //reset_echo_mode();
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

        while(snake_find_body(snake_body , save_node , cmp_node) != NULL )
        {
            newtag = get_tag();
            tagx = newtag.x;
            tagy = newtag.y;
            printf("%d %d\n",tagx,tagy);

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

int check_point(LLIST *snake_body, snake_node *node)
{
    if (snake_find_body(snake_body, node , cmp_node) != NULL || node->x == 0 || node->x == M-1 || node->y ==0 || node->y == N-1)
    {
        return 0;
    }
    return 1;
}

void print(const LLIST *snake_body)
{
    int i, j;
    for (i=0; i<M; ++i)
    {
        for (j=0; j<N; ++j)
        {
            snake_node *find = (snake_node *)malloc(sizeof(snake_node));
            find->x = i;
            find->y = j;

            if (i==0 || j==0 || i==M-1 || j==N-1)
            {
                printf("#");
            }
            else if (snake_find_body(snake_body, find , cmp_node) != NULL)
            {
                printf("O");
            }
            else if (i==tagx && j==tagy)
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

