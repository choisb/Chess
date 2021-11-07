#include "Game.h"
#include "Actor.h"
#include <algorithm>

Game::Game()
    : mTicksCount{}
    , mIsRunning{true}
    , mUpdatingActors{false}
{

}
Game::~Game()
{
    // 삭제할 Actor들을 임시 벡터로 이동
    std::vector<Actor*> deleteActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deleteActors.emplace_back(actor);
        }
    }

    // 엑터들 삭제
    for (auto actor : deleteActors)
    {
        delete actor;
    }
}
bool Game::Initialize()
{
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Create an SDL Window
    mWindow = SDL_CreateWindow(
        "Chess 2D", // Window title
        100,	// Top left x-coordinate of window
        100,	// Top left y-coordinate of window
        1024,	// Width of window
        768,	// Height of window
        0		// Flags (0 for no flags set)
    );

    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create SDL renderer
    mRenderer = SDL_CreateRenderer(
        mWindow,  // Window to create renderer for
        -1,		        // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    
    LoadData();

    return true;
}
void Game::LoadData()
{
    Actor* a = new Actor(*this);
    a = new Actor(*this);
    a = new Actor(*this);
}
void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
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
}
void Game::GenerateOutput()
{
    // Set draw color to blue
    SDL_SetRenderDrawColor(
        mRenderer,
        0,		// R
        0,		// G 
        255,	// B
        255		// A
    );

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
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

    // 죽은 Actor들을 임시 벡터로 이동
    std::vector<Actor*> deadActors;
    for (auto actor : mActors)
    {
        if (actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }

    // 죽은 액터 제거(mActors에서 추려낸 액터들)
    for (auto actor : deadActors)
    {
        delete actor;
    }

}

void Game::AddActor(Actor* actor)
{
    if (mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    }
    else
    {
        mActors.emplace_back(actor);
    }
}
void Game::RemoveActor(Actor* target)
{
    const auto iter = std::find(mActors.begin(), mActors.end(), target);
    // mActors에 Remove할 Actor가 존재한다면 swap and pop
    if (iter != mActors.end())
    {
        std::iter_swap(iter, std::prev(mActors.end()));
        mActors.pop_back();
    }
}
