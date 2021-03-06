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
    // 兄希君 持失
    mRenderer = std::make_shared<Renderer>(*this);
    if (mRenderer->Initialize() == false) return false;
    
    // Game Mode 持失
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
    // GameManager 飴重
    mGameManager->UpdateGame(deltaTime);
}
void Game::UpdateAllActors(float deltaTime)
{
    // Actor級 Updated
    mUpdatingActors = true;
    for (auto& actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    // 企奄掻戚揮 Actor級聖 mActors 困斗稽 戚疑
    for (auto& actor : mPendingActors)
    {
        mActors.emplace_back(actor);
    }
    mPendingActors.clear();

    //宋精 Actor級 Shutdown
    for (auto& actor : mActors)
    {
        if (actor->GetState() == Actor::State::EDead)
        {
            actor->Shutdown();
        }
    }

    // 宋精 Actor級 貼事 板 薦暗
    auto iter = std::remove_if(mActors.begin(), mActors.end(),[](const std::shared_ptr<Actor>& actor)
    {
        return actor->GetState() == Actor::State::EDead;
    });
    // 宋精 植斗級 壕伸拭辞 薦暗
    mActors.erase(iter, mActors.end());
    
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
