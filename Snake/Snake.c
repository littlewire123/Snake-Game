#include "./Snake.h"

LLIST *snake_init_body(LLIST* snake_body_init, int lenth)
{
    snake_body_init = llist_creat(sizeof(snake_node));

    snake_node node;
    int i=5;
    node.x = i;
    node.y = 6;

    snake_insert_body(snake_body_init,&node);
    node.y = 7;
    snake_insert_body(snake_body_init,&node);
    node.y = 8;
    snake_insert_body(snake_body_init,&node);

    return snake_body_init;
}

void snake_insert_body(LLIST* snake_body_insert, snake_node *node)
{
    llist_append(snake_body_insert, node);
    return ;
}

NODE *snake_find_body(LLIST* snake_body_find,const snake_node *node , llist_cmp_t cmp)
{
    NODE *curret = snake_body_find->head.next;

    while(curret != &snake_body_find->head)
    {
        if(cmp(curret->data , node) == 0)
        {   
            
            return curret;
        }
        curret = curret->next;
    }

    return NULL;
}

NODE *snake_find_obstacle(LLIST* snake_obstacle_find,const  snake_node *node , llist_cmp_t cmp)
{
    NODE *curret = snake_obstacle_find->head.next;

    while(curret != &snake_obstacle_find->head)
    {
        if(cmp(curret->data , node) == 0)
        {   
                 
            return curret;
        }
        curret = curret->next;
    }
    return NULL;
}

NODE *snake_find_tag(LLIST* snake_tag_find , const snake_node *node , llist_cmp_t cmp)
{
    NODE *curret = snake_tag_find->head.next;

    while(curret != &snake_tag_find->head)
    {
        if(cmp(curret->data , node) == 0)
        {
            return curret;
        }
        curret = curret->next;
    }
    return NULL;
}
