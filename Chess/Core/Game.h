#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "SDL/SDL.h"

class Actor;
class SpriteComponent;

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

    // 새로운 Actor를 생성하는 함수
    template<class T, class... Param>
    std::weak_ptr<T> CreateActor(Param&&... _Val);

    SDL_Texture* GetTexture(const std::string& fileName);

    void AddSpriteToArray(const std::shared_ptr<SpriteComponent>& spriteComponent);

private:
    void AddActorToArray(const std::shared_ptr<Actor>& actor);

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
    // Sprite 공유 포인터를 담고 있는 배열
    std::vector<std::shared_ptr<SpriteComponent>> mSpriteComponents;

    // 텍스처 load를 위한 Map
    std::unordered_map<std::string, SDL_Texture*> mTextures;
    // 게임을 운영하기 위한 Game mode 
    std::unique_ptr<class GameManager> mGameManager;
};

template<class T, class... Param>
std::weak_ptr<T> Game::CreateActor(Param&&... _Args)
{
    // Actor의 파생클래스가 아닐경우 예외처리
    static_assert(std::is_base_of<Actor, T>::value, "Template argument T must be a derived class from the Actor class");

    std::shared_ptr<T> tSharedPtr = std::make_shared<T>(std::forward<Param>(_Args)...);
    std::weak_ptr<T> tWeakPtr = tSharedPtr;
    std::shared_ptr<Actor> actorSharedPtr = std::static_pointer_cast<Actor>(tSharedPtr);
    actorSharedPtr->Initialize();
    AddActorToArray(actorSharedPtr);
    return tWeakPtr;
}