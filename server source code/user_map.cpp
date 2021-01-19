#include "user_map.h"

namespace CUSTOM
{
    bool UserMap::isNameOccupied(std::string name)
    {
        // true if name is occupied
        return !(userMap.find(name) == userMap.end());
    }

    User* UserMap::getUser(std::string name)
    {
        if (isNameOccupied(name))
            return userMap[name];
        return nullptr;
    }

    bool UserMap::registerUser(std::string user_info)
    {
        // Parse UserAccountName
        std::size_t dataStartAt = user_info.find("#");
        std::string name = user_info.substr(0, dataStartAt);

        if (isNameOccupied(name))
            return false;
        
        // exclude '\n'
        std::string balance = user_info.substr(dataStartAt + 1, user_info.length() - dataStartAt - 1);
        User* user = new User(name, std::stoi(balance));
        userMap[name] = user;

        return true;
    }

    std::string UserMap::listUser(std::string balance)
    {
        std::string list = balance + "\n";
        list += std::to_string(onlineList.size()) + "\n";
        for (auto& user : onlineList)
        {
            list += user->getName() + "#";
            list += user->getIP() + "#";
            list += std::to_string(user->getPortNum()) + "\n";
        }
        return list;
    }

    User* UserMap::loginUser(std::string& input_string, Client& client)
    {
        // Parse name
        int foundAt = input_string.find("#");
        std::string name = input_string.substr(0, foundAt);
        User* user;

        // only registerred user can login
        if (isNameOccupied(name)) {
            user = userMap[name];
        }
        else {
            return nullptr;
        }

        // Parse port number and set up user status
        std::string port_num = input_string.substr(foundAt + 1, input_string.length() - foundAt - 1);
        user->setPortNum(std::stoi(port_num));
        user->setIP(client.getIP());

        // Push to online list
        if (!user->getIsOnline())
            onlineList.push_back(user);

        return user;
    }

    bool UserMap::logoutUser(User* user)
    {
        try
        {
            // only if user has logged in can log out
            onlineList.remove(user);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
        user->setOffline();
        return true;
    }

    bool UserMap::transaction(std::string payerName, int amount, std::string payeeName)
    {
        // Promised payer and payee will not exist
        User* payer = getUser(payerName);
        User* payee = getUser(payeeName);
        if (payer->getBalance() - amount < 0)
        {
            std::cout << "Not enought Balance\n";
            return false;
        }
        if (payee->getBalance() + amount > 2147483647) // Max int value
        {
            std::cout << "Money overflow. You are too rich to use our service.\n";
            return false;
        }
        payer->updateBalance(-amount);
        payee->updateBalance(amount);
        std::cout << "payer balance: " << payer->getBalance() << std::endl;
        std::cout << "payee balance: " << payee->getBalance() << std::endl;
        return true;
    }

}