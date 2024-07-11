#include "user.h"


int main()
{
    while(1)
    {
        printf("1 : class_mode \n");
        int mod;
        printf("input your mod :");
        scanf("%d",&mod);

        switch (mod)
        {
        case 1:
            mod = 0;
            start_classic_game();
            break;
        
        default:
            break;
    }


    }
    
    return 0;
}