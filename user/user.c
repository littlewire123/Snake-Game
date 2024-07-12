#include "./user.h"

void snake_destroy()
{
    llist_destroy(&snake_body_classic);
    llist_destroy(&snake_body_challenge);
    llist_destroy(&snake_body_power);
    llist_destroy(&snake_obstacle_challenge);
    llist_destroy(&snake_obstacle_power);
    llist_destroy(&snake_power_tag);
}