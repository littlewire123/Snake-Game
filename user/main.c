#include "user.h"

int main()
{
    while(1)
    {
        snake_body_challenge = snake_init_body(snake_body_challenge , LENGTH);
        snake_obstacle = init_map(snake_obstacle);
        printf("1 : class_mode \n");
        printf("2 : challenge_mode \n");
        printf("5 : exit game \n");
        int mod;
        printf("input your mod :");

        scanf("%d",&mod);

        switch (mod)
        {
        case 1:
            if((snake_body_classic = init_classic_game(snake_body_classic)) == NULL)
            {
                snake_body_classic = snake_init_body(snake_body_classic , LENGTH);
                start_classic_game(snake_body_classic);
            }
            else
            {
                system("clear");
                printf("1 : New game\n");
                printf("2 : Continue  game\n");
                scanf("%d" , &mod);
                if(mod == 1)
                {
                    snake_body_classic = snake_init_body(snake_body_classic , LENGTH);
                    start_classic_game(snake_body_classic);
                }
                else
                {
                    continue_classic_game(snake_body_classic);
                }
            }
            break;
        case 2:
            if((snake_body_challenge = init_challenge_game(snake_body_challenge , &snake_obstacle)) == NULL)
            {
                snake_body_challenge = snake_init_body(snake_body_challenge , LENGTH);
                challenge_mode_start(snake_body_challenge , snake_obstacle);
            }
            else
            {
                system("clear");
                printf("1 : New game\n");
                printf("2 : Continue  game\n");
                scanf("%d" , &mod);
                if(mod == 1)
                {
                    snake_body_challenge = snake_init_body(snake_body_challenge , LENGTH);
                    challenge_mode_start(snake_body_challenge , snake_obstacle);
                }
                else
                {
                    continue_challenge_game(snake_body_challenge,snake_obstacle);
                }
            }
            break;
        default:
            break;
        }
    }
    llist_destroy(&snake_body_classic);
    llist_destroy(&snake_body_challenge);
    llist_destroy(&snake_obstacle);
    
    return 0;
}