#include "user.h"


int main()
{
    while(1)
    {
<<<<<<< HEAD
        printf("1 : class_mode \n");
        int mod;
        printf("input your mod :");
=======
        printf("input your mod :");
        int mod;
>>>>>>> jyg
        scanf("%d",&mod);

        switch (mod)
        {
        case 1:
<<<<<<< HEAD
            mod = 0;
            start_classic_game();
=======
            break;
        case 2:
            challenge_mode_start();
>>>>>>> jyg
            break;
        
        default:
            break;
<<<<<<< HEAD
    }


=======
        }
        printf("1");
>>>>>>> jyg
    }
   
    
    return 0;
}