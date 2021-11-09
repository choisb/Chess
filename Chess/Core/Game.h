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

    // ���ο� Actor�� �����ϴ� �Լ�
    template<class T, class... Param>
    std::weak_ptr<T> CreateActor(Param&&... _Val);

    SDL_Texture* GetTexture(const std::string& fileName);

    void AddSpriteToArray(const std::shared_ptr<SpriteComponent>& spriteComponent);

private:
    void AddActorToArray(const std::shared_ptr<Actor>& actor);

    // ���ӿ� �ʿ��� data�� �ε�. Initialize �Լ����� ȣ���
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
    // Actor���� Update ���� ��� true 
    bool mUpdatingActors;


    // Actor ���� �����͸� ��� �ִ� �迭
    std::vector<std::shared_ptr<Actor>> mActors;
    std::vector<std::shared_ptr<Actor>> mPendingActors;
    // Sprite ���� �����͸� ��� �ִ� �迭
    std::vector<std::shared_ptr<SpriteComponent>> mSpriteComponents;

    // �ؽ�ó load�� ���� Map
    std::unordered_map<std::string, SDL_Texture*> mTextures;
    // ������ ��ϱ� ���� Game mode 
    std::unique_ptr<class GameManager> mGameManager;
};

template<class T, class... Param>
std::weak_ptr<T> Game::CreateActor(Param&&... _Args)
{
    // Actor�� �Ļ�Ŭ������ �ƴҰ�� ����ó��
    static_assert(std::is_base_of<Actor, T>::value, "Template argument T must be a derived class from the Actor class");

    std::shared_ptr<T> tSharedPtr = std::make_shared<T>(std::forward<Param>(_Args)...);
    std::weak_ptr<T> tWeakPtr = tSharedPtr;
    std::shared_ptr<Actor> actorSharedPtr = std::static_pointer_cast<Actor>(tSharedPtr);
    actorSharedPtr->Initialize();
    AddActorToArray(actorSharedPtr);
    return tWeakPtr;
}