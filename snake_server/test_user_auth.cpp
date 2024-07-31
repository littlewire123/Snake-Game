#include "./user_auth/user_auth.hpp"

int main()
{
    user_auth auth;
    user_info_t user = {"testuser", "testpassword"};

    if (auth.Register(user))
    {
        std::cout << "Registration successful.\n";
    }
    else
    {
        std::cout << "Registration failed.\n";
    }

    if (auth.Login(user))
    {
        std::cout << "Login successful.\n";
    }
    else
    {
        std::cout << "Login failed.\n";
    }

    return 0;
}