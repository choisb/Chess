#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include "SDL/SDL.h"

class Actor;
class SpriteComponent;
class Renderer;
class GameManager;

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

    void IncreaseCreatedActorCount() { mCreatedActorCount++; }
    void IncreaseCreatedComponentCount() { mCreatedComponentCount++; }
    void IncreaseDestroyedActorCount() { mDestroyedActorCount++; }
    void IncreaseDestroyedComponentCount() { mDestroyedComponentCount++; }
    // ���ο� Actor�� �����ϴ� �Լ�
    template<class T, class... Param>
    std::weak_ptr<T> CreateActor(Param&&... _Val);

    // Getter & Setter
    std::shared_ptr<Renderer> GetRenderer() const { return mRenderer; }
private:
    // ������ Actro�� Actor �迭�� �����ϴ� �Լ�
    void AddActorToArray(const std::shared_ptr<Actor>& actor);
    // ��� Actor���� �����ϴ� �Լ�
    void UpdateAllActors(float deltaTime);
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    // Actor���� Update ���� ��� true 
    bool mUpdatingActors;

    // Actor ���� �����͸� ��� �ִ� �迭
    std::vector<std::shared_ptr<Actor>> mActors;
    std::vector<std::shared_ptr<Actor>> mPendingActors;

    // ������ ��ϱ� ���� Game Manager ������
    std::unique_ptr<GameManager> mGameManager;
    // ������ ������
    std::shared_ptr<Renderer> mRenderer;

    // �� ������ Actor�� ��
    int mCreatedActorCount{};
    // �� ������ Component�� ��
    int mCreatedComponentCount{};
    // �� ������ Actor�� ��
    int mDestroyedActorCount{};
    // �� ������ Component�� ��
    int mDestroyedComponentCount{};

    // ���� ������ ���콺 ��ư ����
    Uint32 mPrevButtons;
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