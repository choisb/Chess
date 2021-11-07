#pragma once
#include "SDL/SDL.h"
#include <memory>
#include <vector>
class Game
{
public:
    Game();
    ~Game();
    // Initialize the game
    bool Initialize();
    // Runs the game loop until the game is over
    void RunLoop();
    // Shutdown the game
    void Shutdown();

    std::weak_ptr<class Actor> SpawnActor();

private:
    void AddActorToArray(std::shared_ptr<Actor> actor);
    //void RemoveActor(Actor* actor);

    // 게임에 필요한 data들 로딩. Initialize 함수에서 호출됨
    void LoadData();

    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();


    // Window created by SDL
    SDL_Window* mWindow;
    // Renderer for 2D drawing
    SDL_Renderer* mRenderer;
    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    // Actor들이 Update 중인 경우 true 
    bool mUpdatingActors;

    // Actor 원본 포인터를 담고 있는 배열
    std::vector<std::shared_ptr<Actor>> mActors;
    std::vector<std::shared_ptr<Actor>> mPendingActors;
};
