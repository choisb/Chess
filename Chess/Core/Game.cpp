#include <algorithm>

#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "Source/GameManager.h"
Game::Game()
    : mTicksCount{}
    , mIsRunning{true}
    , mUpdatingActors{false}
    , mPrevButtons(0)
{

}
Game::~Game()
{

}
bool Game::Initialize()
{
    // 렌더러 생성
    mRenderer = std::make_shared<Renderer>(*this);
    if (mRenderer->Initialize() == false) return false;
    
    // Game Mode 생성
    mGameManager = std::make_unique<GameManager>(*this);
    mGameManager->Initialize();
    return true;
}

void Game::Shutdown()
{
    mGameManager->Shutdown();
    mRenderer->Shutdown();
    mActors.clear();
    mPendingActors.clear();
    SDL_Log("Created Actor Count: %d", mCreatedActorCount);
    SDL_Log("Destroyed Actor Count: %d", mDestroyedActorCount);    
    SDL_Log("Created Component Count: %d", mCreatedComponentCount);
    SDL_Log("Destroyed Component Count: %d", mDestroyedComponentCount);
    SDL_Quit();
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}
void Game::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // If we get an SDL_QUIT event, end loop
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    // Get state of keyboard
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // If escape is pressed, also end loop
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    // Get state of mouse
    int x, y;
    const Uint32 currbuttons = SDL_GetMouseState(&x, &y);
    if ((currbuttons & SDL_BUTTON_LMASK) != 0 && (mPrevButtons & SDL_BUTTON_LMASK) == 0)
    {
        mGameManager->LeftClickDown(Coordinates2(x, y));
    }

    mPrevButtons = currbuttons;
}
void Game::GenerateOutput()
{
    mRenderer->Draw();
}
void Game::UpdateGame()
{
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // Delta time is the difference in ticks from last frame
    // (converted to seconds)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // Clamp maximum delta time value
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    // Update tick counts (for next frame)
    mTicksCount = SDL_GetTicks();

    UpdateAllActors(deltaTime);
    // GameManager 갱신
    mGameManager->UpdateGame(deltaTime);
}
void Game::UpdateAllActors(float deltaTime)
{
    // Actor들 Updated
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    // 대기중이던 Actor들을 mActors 벡터로 이동
    for (auto actor : mPendingActors)
    {
        mActors.emplace_back(actor);
    }
    mPendingActors.clear();

    // 죽은 Actor들을 삭제 
    mActors.erase(std::remove_if(mActors.begin(), mActors.end(),
        [](const std::shared_ptr<Actor>& actor)
    {
        return actor->GetState() == Actor::State::EDead;
    }), mActors.end());
}


void Game::AddActorToArray(const std::shared_ptr<Actor>& actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.push_back(std::move(actor));
    }
    else
    {
        mActors.push_back(std::move(actor));
    }
}
