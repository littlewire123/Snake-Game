#include "./game_logic/game_logic.hpp"
#include <time.h>
using namespace std;
int main()
{
    srand(time(NULL));
    game_logic game(game_logic::CHALLENGE);
    

    if (!game.init_game())
    {
        cout<<"game init failed\n";
    }
    int i = 0;
    cout<<"244"<<endl;

    while (game.add_user(i))
    {
        ++i;
        cout<<"123"<<endl;
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