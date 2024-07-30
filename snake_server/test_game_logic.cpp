#include "./game_logic/game_logic.hpp"
#include <time.h>
#include <iostream>
#include <vector>
#include <map>
using namespace std;

int main()
{
    srand(time(NULL));

    /*while (game.add_user(i))
    {
        ++i;
        cout<<"123"<<endl;
    }*/
    map<int, game_logic> m;
    pair<int, game_logic> p = make_pair(1, game_logic(game_logic::POWER_UP));
    m.insert(p);
    cout << "333" << endl;

    if (!m[1].init_game())
    {
        cout << "game init failed\n";
    }
    int i = 0;
    cout << "244" << endl;

    m[1].add_user(1, 2);

    m[1].print();
    getchar();

    while (m[1].move())
    {
        m[1].print();
        getchar();
    }

    m[1].end_game();
    return 0;
}