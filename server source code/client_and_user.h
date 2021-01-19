#ifndef _CLIENT_AND_USER_H_
#define _CLIENT_AND_USER_H_

#include <iostream>
#include <string>
#include <sys/socket.h>

#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace CUSTOM
{
    class Client
    {
    private:
        std::string ip_addr;
        int socket;
        SSL* ssl;
    public:
        Client();
        Client(std::string ip_addr, int socket, SSL* ssl);

        int getSocket();
        SSL* getSSLSocket();
        std::string getIP();
    };

    class User
    {
    private:
        std::string name;
        std::string ip_addr;
        int port_num;
        int balance;
        bool isOnline;
    public:
        User();
        User(std::string, int);

        /* ===getters and setters=== */
        std::string getName();
        void setName(std::string);

        std::string getIP();
        void setIP(std::string);

        int getPortNum();
        void setPortNum(int);

        int getBalance();
        void updateBalance(int);

        void setOnline();
        void setOffline();

        bool getIsOnline();
        /* ===getters and setters=== */
    };
}


#endif