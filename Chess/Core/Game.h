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
    // 새로운 Actor를 생성하는 함수
    template<class T, class... Param>
    std::weak_ptr<T> CreateActor(Param&&... _Val);

    // Getter & Setter
    std::shared_ptr<Renderer> GetRenderer() const { return mRenderer; }
private:
    // 생성된 Actro를 Actor 배열에 삽입하는 함수
    void AddActorToArray(const std::shared_ptr<Actor>& actor);
    // 모든 Actor들을 갱신하는 함수
    void UpdateAllActors(float deltaTime);
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Number of ticks since start of game
    Uint32 mTicksCount;
    // Game should continue to run
    bool mIsRunning;
    // Actor들이 Update 중인 경우 true 
    bool mUpdatingActors;

    // Actor 원본 포인터를 담고 있는 배열
    std::vector<std::shared_ptr<Actor>> mActors;
    std::vector<std::shared_ptr<Actor>> mPendingActors;

    // 게임을 운영하기 위한 Game Manager 포인터
    std::unique_ptr<GameManager> mGameManager;
    // 렌더러 포인터
    std::shared_ptr<Renderer> mRenderer;

    // 총 생성된 Actor의 수
    int mCreatedActorCount{};
    // 총 생성된 Component의 수
    int mCreatedComponentCount{};
    // 총 삭제된 Actor의 수
    int mDestroyedActorCount{};
    // 총 삭제된 Component의 수
    int mDestroyedComponentCount{};

    // 이전 프레임 마우스 버튼 상태
    Uint32 mPrevButtons;
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