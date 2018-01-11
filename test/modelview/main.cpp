#include "game.h"

#include <exception>
#include <iostream>
#include <string>

using namespace Reweng;


int main()
{
    try
    {
        Game game;
        game.Start();

        

        volatile bool flag = true;
        while (flag)
        {
            
        }

        
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }
}