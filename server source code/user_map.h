#ifndef _USER_MAP_H_
#define _USER_MAP_H_

#include <unordered_map>
#include <list>
#include "client_and_user.h"

namespace CUSTOM
{
    class UserMap
    {
    private:
        std::unordered_map<std::string, User*> userMap;
        std::list<User*> onlineList;
    public:
        bool isNameOccupied(std::string);
        User* getUser(std::string);
        bool registerUser(std::string);
        std::string listUser(std::string);
        User* loginUser(std::string&, Client&);
        bool logoutUser(User* user);
        bool transaction(std::string, int, std::string);
    };
}

#endif