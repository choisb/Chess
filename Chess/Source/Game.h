#pragma once
#include "SDL/SDL.h"
#include <memory>
class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();


    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTickCount;
    bool mIsRunning;


};

