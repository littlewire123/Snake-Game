#include "user.h"

void ls(const void *data)
{
    printf("%d %d\n", ((snake_node *)data)->x , ((snake_node *)data)->y);
}

int main()
{
    srand(time(NULL));
    while(1)
    {
        printf("1 : class_mode \n");
        printf("2 : challenge_mode \n");
        printf("3 : power_up_mode \n");
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
            if((snake_body_challenge = init_challenge_game(snake_body_challenge , &snake_obstacle_challenge)) == NULL)
            {
                snake_body_challenge = snake_init_body(snake_body_challenge , LENGTH);
                snake_obstacle_challenge = init_map(snake_body_challenge,snake_obstacle_challenge);
                challenge_mode_start(snake_body_challenge , snake_obstacle_challenge);
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
                    snake_obstacle_challenge = init_map(snake_body_challenge,snake_obstacle_challenge);
                    challenge_mode_start(snake_body_challenge , snake_obstacle_challenge);
                }
                else
                {
                    continue_challenge_game(snake_body_challenge,snake_obstacle_challenge);
                }
            }
            break;
        case 3:
            if((snake_body_power = init_power_game(snake_body_power,&snake_obstacle_power,&snake_tag_power)) == NULL)
            {
                snake_body_power = snake_init_body(snake_body_power , LENGTH);
                snake_obstacle_power = init_map(snake_body_power ,snake_obstacle_power);
                snake_tag_power = init_tag(snake_body_power , snake_tag_power , snake_obstacle_power);
                power_mode_start(snake_body_power , snake_obstacle_power , snake_tag_power);
            }
            else
            {
                system("clear");
                printf("1 : New game\n");
                printf("2 : Continue  game\n");
                scanf("%d" , &mod);
                if(mod == 1)
                {
                    snake_body_power = snake_init_body(snake_body_power , LENGTH);
                    snake_obstacle_power = init_map(snake_body_power ,snake_obstacle_power);
                    snake_tag_power = init_tag(snake_body_power , snake_tag_power , snake_obstacle_power);
                    power_mode_start(snake_body_power , snake_obstacle_power , snake_tag_power);
                }
                else
                {
                    continue_power_game(snake_body_power,snake_obstacle_power,snake_tag_power);
                }
            }
        case 4:
            break;
        case 5:   
            About_Game();  
            break;
            
        default:
            break;
        }
    }
    snake_destroy();

    return 0;
}