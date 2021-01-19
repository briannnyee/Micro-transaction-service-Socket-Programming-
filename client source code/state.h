#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

namespace CUSTOM
{
    class STATE
    {
    protected:
        //std::stack<int> state_seq;
        std::vector<std::string> state_feature;
        std::string activated_msg;

    public:
        STATE();
        ~STATE() {}

        void printFeature();
        std::string runFeature(int);
        std::string queryFeature(int);

        virtual std::string microTransaction(std::string, std::string, std::string, int) {}
        virtual void startListen(std::string, SSL*) {}
    };
}

#endif