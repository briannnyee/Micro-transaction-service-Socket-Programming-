#ifndef _TCP_CLIENT_FSM_H_
#define _TCP_CLIENT_FSM_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

// openSSL header files
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>

#include "FSM.h"
#include "myUIUX.h"
#include "appSpecConst.h"

namespace CUSTOM
{
    class ClientFSM: public FSM
    {
    private:
        std::string CRLF = "\r\n";
    public:
        ClientFSM();
        ~ClientFSM() {}
        std::string processRequest(int);
        void refreshInterface(int);
    };

    class WELCOME: public STATE
    {
    public:
        WELCOME();
        ~WELCOME() {}
    };
    
    class LOGGED_IN: public STATE
    {
    private:
        std::string user_name;
        std::string port_str;
        int port_num;
    public:
        LOGGED_IN(std::string, std::string, int);
        ~LOGGED_IN() {}

        std::string microTransaction(std::string, std::string, std::string, int);
        void startListen(std::string, SSL*);
    };
}


#endif