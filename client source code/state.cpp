#include "state.h"
#include "myUIUX.h"

namespace CUSTOM
{
    STATE::STATE()
    {

    }

    void STATE::printFeature()
    {
        printSeperator();
        std::cout << activated_msg;
        printSeperator();
        for(int i = 0; i < state_feature.size(); i++)
        {
            std::cout << i + 1 << " -> " << state_feature[i] << std::endl;
        }
    }
    
    std::string STATE::queryFeature(int featureIndex)
    {
        if (featureIndex > state_feature.size() || featureIndex < 1) {
            return "Error 1487";
        }
        //std::cout << "8\n";
        return state_feature[featureIndex - 1];
    }
}

