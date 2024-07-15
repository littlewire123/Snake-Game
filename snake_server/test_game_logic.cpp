#include "user.h"
#include <time.h>
using namespace std;
int main()
{
    srand(time(NULL));
    game_logic game;

    if (!game.init_game())
    {
        cout<<"game init failed\n";
    }
    int i = 0;

    while (game.add_user(i))
    {
        ++i;
    }

    //game.add_user(1);

    game.print();
    getchar();

    while (game.move())
    {
        game.print();
        getchar();
    }

    game.end_game();
    return 0;
}