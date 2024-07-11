#include "user.h"


int main()
{
    while(1)
    {
        printf("1 : class_mode \n");
        printf("2 : challenge_mode \n");
        int mod;
        printf("input your mod :");

        scanf("%d",&mod);

        switch (mod)
        {
        case 1:
            start_classic_game();
            break;
        case 2:
            challenge_mode_start();
            break;
        
        default:
            break;
        }


    }

    
    return 0;
}