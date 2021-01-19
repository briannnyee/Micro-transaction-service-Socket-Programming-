#include "client_and_user.h"

namespace CUSTOM
{
    Client::Client()
    {
        ip_addr = "NULL";
        socket = 0;
        ssl = nullptr;
    }

    Client::Client(std::string ip_addr, int socket, SSL* ssl)
    {
        this->ip_addr = ip_addr;
        this->socket = socket;
        this->ssl = ssl;
    }

    int Client::getSocket()
    {
        return socket;
    }

    SSL* Client::getSSLSocket()
    {
        return ssl;
    }

    std::string Client::getIP()
    {
        return ip_addr;
    }

    User::User()
    {
        name = "NULL";
        ip_addr = "NULL";
        port_num = -1;
        balance = -1;
        isOnline = false;
    }
    User::User(std::string name, int balance)
    {
        this->name = name;
        this->ip_addr = "NULL";
        this->port_num = -1;
        this->balance = balance;
        this->isOnline = false;
    }

    std::string User::getName()
    {
        return this->name;
    }

    void User::setName(std::string name)
    {
        this->name = name;
    }

    std::string User::getIP()
    {
        return this->ip_addr;
    }

    void User::setIP(std::string IP)
    {
        this->ip_addr = IP;
    }

    int User::getPortNum()
    {
        return this->port_num;
    }

    void User::setPortNum(int port_num)
    {
        this->port_num = port_num;
    }

    int User::getBalance()
    {
        return this->balance;
    }

    void User::updateBalance(int amount)
    {
        this->balance += amount;
    }

    void User::setOnline()
    {
        this->isOnline = true;
    }

    void User::setOffline()
    {
        this->isOnline = false;
    }

    bool User::getIsOnline()
    {
        return this->isOnline;
    }
}