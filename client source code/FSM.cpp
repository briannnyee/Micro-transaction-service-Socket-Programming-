#include "FSM.h"

namespace CUSTOM
{
    FSM::FSM() 
    {
        std::cout << activated_msg;
    }

    FSM::~FSM() 
    {
        
    }

    void FSM::addState(StateRef target) 
    {
        state_seq.push( std::move(target) );
    }

    void FSM::removeState() 
    {
        if (!state_seq.empty()) {
            state_seq.pop();
        }   
    }

    StateRef &FSM::getActiveState() 
    {
        if (!state_seq.empty()) {
            return state_seq.top();
        }
    }
}



