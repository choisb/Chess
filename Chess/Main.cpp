#include "Game.h"
#include <iostream>

int main(int argc, char** argv) 
{
    Game game;

    bool isSucceed = game.Initialize();
    if (isSucceed)
    {
        game.RunLoop();
    }
    game.Shutdown();
    std::cin.get();

    return 0;
}