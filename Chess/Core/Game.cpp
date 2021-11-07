#include "Game.h"
#include "Actor.h"
#include <algorithm>

int test;
Game::Game()
    : mTicksCount{}
    , mIsRunning{true}
    , mUpdatingActors{false}
{

}
Game::~Game()
{
    // mActors 배열을 비운다. (shared_ptr 이기 때문에 자동으로 삭제)
    mActors.clear();

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
    auto a = SpawnActor();
    a = SpawnActor(); 
    a = SpawnActor();
}
std::weak_ptr<Actor> Game::SpawnActor()
{
    std::weak_ptr<Actor> actorWeakPtr;
    std::shared_ptr<Actor> actorSharedPtr = std::make_shared<Actor>(*this);
    AddActorToArray(actorSharedPtr);
    actorWeakPtr = actorSharedPtr;
    return actorWeakPtr;
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

    // 죽은 Actor들을 삭제 
    mActors.erase(std::remove_if(mActors.begin(), mActors.end(), 
        [](const std::shared_ptr<Actor>& actor)
        {
            return actor->GetState() == Actor::EDead;
        }), mActors.end());
}

void Game::AddActorToArray(std::shared_ptr<Actor> actor)
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
