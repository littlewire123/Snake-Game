#include "user.h"

int main()
{
    try
    {
        TcpServer server("0.0.0.0", 8080);
        if (!server.bindAndListen())
        {
            return 1;
        }
        server.run();
    }
    catch (exception &e)
    {
        cout << e.what() <<endl;
    }
    return 0;
}
