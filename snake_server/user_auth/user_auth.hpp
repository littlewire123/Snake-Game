#ifndef USER_AUTH_HPP
#define USER_AUTH_HPP

#include <fstream>
#include <cstring>
#include <crypt.h>
#include <iostream>
#include "../protocol/protocol.hpp"

class user_auth
{
public:
    user_auth(const std::string salt = "littlewire"):SALT(salt), filename("./user_auth/users.bin"){}
    ~user_auth() = default;
    bool Register(const user_info_t &userInfo);
    bool Login(const user_info_t &userInfo);
private:
    const std::string SALT;
    const std::string filename;
};

bool user_auth::Register(const user_info_t &userInfo)
{
    std::ifstream infile(filename, std::ios::binary);
    if (infile)
    {
        user_info_t existingUser;
        while (infile.read(reinterpret_cast<char *>(&existingUser), sizeof(user_info_t)))
        {
            if (std::strcmp(existingUser.user_name, userInfo.user_name) == 0)
            {
                std::cout << "User already exists.\n";
                infile.close();
                return false;
            }
        }
        infile.close();
    }

    std::ofstream outfile(filename, std::ios::binary | std::ios::app);
    if (!outfile)
    {
        std::cerr << "Failed to open file for writing.\n";
        return false;
    }

    user_info_t userToWrite = userInfo;
    std::string encryptedPwd = crypt(userInfo.user_pwd, SALT.c_str());
    std::strncpy(userToWrite.user_pwd, encryptedPwd.c_str(), sizeof(userToWrite.user_pwd) - 1);
    outfile.write(reinterpret_cast<const char *>(&userToWrite), sizeof(user_info_t));
    outfile.close();
    return true;
}

bool user_auth::Login(const user_info_t &userInfo)
{
    std::ifstream infile(filename, std::ios::binary);
    if (!infile)
    {
        std::cerr << "Failed to open file for reading.\n";
        return false;
    }

    user_info_t existingUser;
    while (infile.read(reinterpret_cast<char *>(&existingUser), sizeof(user_info_t)))
    {
        if (std::strcmp(existingUser.user_name, userInfo.user_name) == 0)
        {
            std::string encryptedPwd = crypt(userInfo.user_pwd, SALT.c_str());
            if (std::strcmp(existingUser.user_pwd, encryptedPwd.c_str()) == 0)
            {
                infile.close();
                return true;
            }
            else
            {
                infile.close();
                return false;
            }
        }
    }

    infile.close();
    return false;
}

#endif