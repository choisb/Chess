#include <algorithm>

#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "SDL/SDL_image.h"
#include "Source/GameManager.h"
Game::Game()
    : mTicksCount{}
    , mIsRunning{true}
    , mUpdatingActors{false}
{

}
Game::~Game()
{
    // mActors �迭�� ����. (shared_ptr �̱� ������ �ڵ����� ����)
    mActors.clear();
    mSpriteComponents.clear();

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
        768,	// Width of window
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
    
    // Game Mode ����
    mGameManager = std::make_unique<GameManager>(*this);

    LoadData();
    return true;
}
void Game::LoadData()
{

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
        100,		// R
        100,		// G 
        100,	// B
        255		// A
    );

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    for (auto sprite : mSpriteComponents)
    {
        sprite->Draw(mRenderer);
    }
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

    // Actor�� Updated
    mUpdatingActors = true;
    for (auto actor : mActors)
    {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    // ������̴� Actor���� mActors ���ͷ� �̵�
    for (auto actor : mPendingActors)
    {
        mActors.emplace_back(actor);
    }
    mPendingActors.clear();

    // ���� Actor���� ���� 
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
// �׷��� ������ Ž���Ͽ� ����
void Game::AddSpriteToArray(std::shared_ptr<SpriteComponent> spriteComponent)
{
    auto iter = mSpriteComponents.begin();
    for (; iter != mSpriteComponents.end(); iter++)
    {
        if ((*iter)->GetDrawOrder() > spriteComponent->GetDrawOrder())
            break;
    }
    mSpriteComponents.insert(iter, spriteComponent);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
    // ��ȯ�� texture�� �ּҰ��� ���� ����
    SDL_Texture* tex = nullptr;

    // unordered_map �����̳ʿ� ����Ǿ� �ִ� mTextures���� fileName���� �˻�
    auto iter = mTextures.find(fileName);
    // fileName�� �ش��ϴ� texture�� �����Ѵٸ�
    if (iter != mTextures.end())
    {
        tex = iter->second;
    }
    else
    {
        // ���Ϸκ��� �ε�
        SDL_Surface* surf = IMG_Load(fileName.c_str());
        if (!surf)
        {
            SDL_Log("Failed to load texture file %s", fileName.c_str());
            return nullptr;
        }

        // �ؽ�ó ����
        tex = SDL_CreateTextureFromSurface(mRenderer, surf);
        SDL_FreeSurface(surf);
        if (!tex)
        {
            SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
            return nullptr;
        }

        mTextures.emplace(fileName.c_str(), tex);
    }
    return tex;
}