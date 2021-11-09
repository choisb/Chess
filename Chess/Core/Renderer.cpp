#include "Renderer.h"
#include "Game.h"
#include "SDL/SDL_image.h"
#include "SpriteComponent.h"

Renderer::Renderer(Game& game)
    : mGame(game)
{

}
Renderer::~Renderer()
{
}
bool Renderer::Initialize()
{
    if (InitializeSDL() == false) return false;
    if (CreateWindow() == false) return false;
    if (CreateSDLRenderer() == false) return false;
    return true;
}
void Renderer::Shutdown()
{
    for (auto& pair : mTextures)
    {
        SDL_DestroyTexture(pair.second);
    }
    mSpriteComponents.clear();
    SDL_DestroyRenderer(mSDLRenderer);
    SDL_DestroyWindow(mWindow);
}
void Renderer::Draw()
{
    // Set draw color to blue
    SDL_SetRenderDrawColor(
        mSDLRenderer,
        100,		// R
        100,		// G 
        100,	// B
        255		// A
    );

    // Clear back buffer
    SDL_RenderClear(mSDLRenderer);

    for (auto sprite : mSpriteComponents)
    {
        sprite->Draw(mSDLRenderer);
    }
    // Swap front buffer and back buffer
    SDL_RenderPresent(mSDLRenderer);
}
bool Renderer::InitializeSDL()
{
    // Initialize SDL
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    return true;
}
bool Renderer::CreateWindow()
{
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
    return true;
}
bool Renderer::CreateSDLRenderer()
{
    // Create SDL renderer
    mSDLRenderer = SDL_CreateRenderer(
        mWindow,  // Window to create renderer for
        -1,		        // Usually -1
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!mSDLRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    return true;
}

// �׷��� ������ Ž���Ͽ� ����
void Renderer::AddSpriteToArray(const std::shared_ptr<SpriteComponent>& spriteComponent)
{
    auto iter = mSpriteComponents.begin();
    for (; iter != mSpriteComponents.end(); iter++)
    {
        if ((*iter)->GetDrawOrder() > spriteComponent->GetDrawOrder())
            break;
    }
    mSpriteComponents.insert(iter, spriteComponent);
}

SDL_Texture* Renderer::GetTexture(const std::string& fileName)
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
        tex = SDL_CreateTextureFromSurface(mSDLRenderer, surf);
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