#ifndef _FSM_H_
#define _FSM_H_

#include <iostream>
#include <stack>
#include <string>
#include <memory>
#include "state.h"

namespace CUSTOM 
{
    typedef std::unique_ptr<STATE> StateRef;
    class FSM
    {
    private:
        std::stack<StateRef> state_seq;
        std::string activated_msg = "System Message: FSM Activated.\n";

    public:
        FSM();
        ~FSM();
        void addState(StateRef);
        void removeState();
        StateRef &getActiveState();
    };
}

#endif