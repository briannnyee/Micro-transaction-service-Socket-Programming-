#include "myUIUX.h"

namespace CUSTOM
{
    void printSeperator()
    {
        for (int i = 0; i < 30; i++)
        {
            std::cout << "=";
        }
        std::cout << std::endl;
    }

    void refreshScreen()
    {
        for (int i = 0; i < 30; i++)
        {
            std::cout << "\n";
        }
    }

    void waitAndRefresh(int duration)
    {
        std::cout << "Refresh in...";
        for (int i = duration; i > 0; i--)
        {
            std::cout << i << "...";
            sleep(1);
        }
        refreshScreen();
    }
}