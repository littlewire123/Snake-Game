#include "./Snake.h"

LLIST *snake_init_body(int lenth)
{
    LLIST *snake_body = llist_creat(sizeof(snake_node));

    snake_node node;
    int i=5;
    node.x = i;
    node.y = 6;

    snake_insert_body(snake_body, &node);
    node.y = 7;
    snake_insert_body(snake_body, &node);
    node.y = 8;
    snake_insert_body(snake_body, &node);

    return snake_body;
}

void snake_insert_body(LLIST *snake_body, snake_node *node)
{
    llist_append(snake_body, node);
    return ;
}

NODE *snake_find_body(const LLIST *snake_body , snake_node *node , llist_cmp_t cmp)
{
    NODE *curret = snake_body->head.next;

    while(curret != &snake_body->head)
    {
        if(cmp(curret->data , node) == 0)
        {   
            return curret;
        }
        curret = curret->next;
    }

    return NULL;
}

NODE *snake_find_obstacle(snake_node *node , llist_cmp_t cmp)
{
    NODE *curret = snake_obstacle->head.next;

    while(curret != &snake_obstacle->head)
    {
        if(cmp(curret->data , node) == 0)
        {   
            return curret;
        }
        curret = curret->next;
    }

    return NULL;
}
