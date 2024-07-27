#include "./user.h"

void snake_destroy()
{
    if (snake_body_classic != NULL)
    {
        llist_destroy(&snake_body_classic);
    }
    if (snake_body_challenge != NULL)
    {
        llist_destroy(&snake_body_challenge);
    }
    if (snake_body_power != NULL)
    {
        llist_destroy(&snake_body_power);
    }
    if (snake_obstacle_challenge != NULL)
    {
        llist_destroy(&snake_obstacle_challenge);
    }
    if (snake_obstacle_power != NULL)
    {
        llist_destroy(&snake_obstacle_power);
    }
    if (snake_tag_power != NULL)
    {
        llist_destroy(&snake_tag_power);
    }
}

void chioce_mod()
{
    srand(time(NULL));
    while (1)
    {
        system("clear");
        print_menu();
        int mod;
        scanf("%d", &mod);
        switch (mod)
        {
        case 1:
            Classic_mod();
            break;
        case 2:
            Challenge_mod();
            break;
        case 3:
            Power_Up_mod();
            break;
        case 4:
            system("clear");
            chioce_online_mod();
            break;
        case 5:
            About_Game();
            break;
        case 6:
            return;
        default:
            while (getchar() != '\n')
            {
            }
            break;
        }
        mod = 0;
    }
    return;
}

void print_menu()
{
    YELLOW_TEXT();
    printf("1 : classic_mode \n");
    printf("2 : challenge_mode \n");
    printf("3 : power_up_mode \n");
    printf("4 : Online_mode\n");
    printf("5 : About_game\n");
    printf("6 : exit game \n");
    printf("input your mod :");
    RESET_TEXT();
}

void print_online_menu()
{
    YELLOW_TEXT();
    printf("1 : classic_online_mode \n");
    printf("2 : challenge_online_mode \n");
    printf("3 : power_up_online_mode \n");
    printf("4 : exit\n");
    printf("input your mod :");
    RESET_TEXT();
}

void Classic_mod()
{
    int mod;
    if ((snake_body_classic = init_classic_game(snake_body_classic)) == NULL)
    {
        snake_body_classic = snake_init_body(snake_body_classic, LENGTH);
        start_classic_game(snake_body_classic);
    }
    else
    {
        system("clear");
        BLUE_TEXT();
        printf("1 : New game\n");
        printf("2 : Continue  game\n");
        printf("3 : Exit \n");
        RESET_TEXT();
        scanf("%d", &mod);
        if (mod == 1)
        {
            snake_body_classic = snake_init_body(snake_body_classic, LENGTH);
            start_classic_game(snake_body_classic);
        }
        else if (mod == 2)
        {
            continue_classic_game(snake_body_classic);
        }
        else
        {
            system("clear");
            return;
        }
    }
    return;
}

void Challenge_mod()
{
    int mod;
    if ((snake_body_challenge = init_challenge_game(snake_body_challenge, &snake_obstacle_challenge)) == NULL)
    {
        snake_body_challenge = snake_init_body(snake_body_challenge, LENGTH);
        snake_obstacle_challenge = init_map(snake_body_challenge, snake_obstacle_challenge);
        challenge_mode_start(snake_body_challenge, snake_obstacle_challenge);
    }
    else
    {
        system("clear");
        BLUE_TEXT();
        printf("1 : New game\n");
        printf("2 : Continue  game\n");
        printf("3 : Exit \n");
        RESET_TEXT();
        scanf("%d", &mod);
        if (mod == 1)
        {
            snake_body_challenge = snake_init_body(snake_body_challenge, LENGTH);
            snake_obstacle_challenge = init_map(snake_body_challenge, snake_obstacle_challenge);
            challenge_mode_start(snake_body_challenge, snake_obstacle_challenge);
        }
        else if (mod == 2)
        {
            continue_challenge_game(snake_body_challenge, snake_obstacle_challenge);
        }
        else
        {
            system("clear");
            return;
        }
    }
}

void Power_Up_mod()
{
    int mod;
    if ((snake_body_power = init_power_game(snake_body_power, &snake_obstacle_power, &snake_tag_power)) == NULL)
    {
        snake_body_power = snake_init_body(snake_body_power, LENGTH);
        snake_obstacle_power = init_map(snake_body_power, snake_obstacle_power);
        snake_tag_power = init_tag(snake_body_power, snake_tag_power, snake_obstacle_power);
        power_mode_start(snake_body_power, snake_obstacle_power, snake_tag_power);
    }
    else
    {
        system("clear");
        BLUE_TEXT();
        printf("1 : New game\n");
        printf("2 : Continue  game\n");
        printf("3 : Exit\n");
        RESET_TEXT();
        scanf("%d", &mod);
        if (mod == 1)
        {
            snake_body_power = snake_init_body(snake_body_power, LENGTH);
            snake_obstacle_power = init_map(snake_body_power, snake_obstacle_power);
            snake_tag_power = init_tag(snake_body_power, snake_tag_power, snake_obstacle_power);
            power_mode_start(snake_body_power, snake_obstacle_power, snake_tag_power);
        }
        else if (mod == 2)
        {
            continue_power_game(snake_body_power, snake_obstacle_power, snake_tag_power);
        }
        else
        {
            system("clear");
            return;
        }
    }
}

void chioce_online_mod()
{
    int mod;
    while (1)
    {
        // system("clear");
        print_online_menu();
        scanf("%d", &mod);
        switch (mod)
        {
        case 1:
            system("clear");
            if (!init_online_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_game();
            }
            break;
        case 2:
            system("clear");
            if (!init_online_challenge_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_challenge_game();
            }
            break;
        case 3:
            system("clear");
            if (!init_online_power_mode())
            {
                printf("connect false !\n");
            }
            else
            {
                start_online_power_game();
            }
            break;
        case 4:
            return;
            break;
        default:
            while (getchar() != '\n')
            {
            }
            continue;
            break;
        }
        mod = 0;
    }
    return;
}

void About_Game()
{
    system("clear");

    printf("蛇的移动\n");
    printf("四方向移动（W:上 S:下 A:左 D:右）\n\n");
    printf("游戏中退出游戏并保存按Q\n\n");
    printf("1 ：经典模式：只有一个食物\n");
    printf("2 ：挑战模式 ：有一个食物和十个障碍物，速度会更快\n");
    printf("3 ：道具模式 ：有十个食物和十个障碍物，吃到食物有概率获取道具效果(加速、减速、增加3个长度、减小3个长度、随机传送)\n");
    printf("4 ：联机模式 ：经典模式下玩家对抗（其他模式联机模式开发中......）\n\n");
    printf("本项目遵循MIT开源协议；\n");
    printf("项目地址：https://github.com/littlewire123/Snake-Game\n\n");

    printf("按任意键退出...\n");
    getchar();
    getchar();
    system("clear");
    return;
}
