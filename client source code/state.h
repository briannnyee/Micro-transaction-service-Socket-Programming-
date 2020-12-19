#ifndef _STATE_H_
#define _STATE_H_

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>

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

        virtual void microTransaction(std::string, std::string, std::string, int) {}
        virtual void startListen(std::string) {}
    };
}

#endif