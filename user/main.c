#include "user.h"



int main()
{
    while(1)
    {
        printf("input your mod :");
        int mod;
        scanf("%d",&mod);

        switch (mod)
        {
        case 1:
            break;
        case 2:
            challenge_mode_start();
            break;
        
        default:
            break;
        }
        printf("1");
    }
   
    

    return 0;
}