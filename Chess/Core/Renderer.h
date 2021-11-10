#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "SDL/SDL.h"

class Game;
class SpriteComponent;

class Renderer
{
public:
    Renderer(Game& game);
    ~Renderer();
    // 초기화 함수
    bool Initialize();
    // 종료 함수. SDL 리소스 정리
    void Shutdown();
    // 출력 함수
    void Draw();
    // Texture 로드 함수
    SDL_Texture* GetTexture(const std::string& fileName);
    // 새로운 Sprite를 배열에 추가하는 함수
    void AddSpriteToArray(const std::shared_ptr<SpriteComponent>& spriteComponent);
    // 배열에서 Sprite를 제거하는 함수
    void RemoveSpriteFromArray(const std::shared_ptr<SpriteComponent>& spriteComponent);
private:

    bool InitializeSDL();
    bool CreateWindow();
    bool CreateSDLRenderer();

    // Game
    Game& mGame;
    // Window created by SDL
    SDL_Window* mWindow;
    // Renderer for 2D drawing
    SDL_Renderer* mSDLRenderer;

    // Sprite 공유 포인터를 담고 있는 배열
    std::vector<std::shared_ptr<SpriteComponent>> mSpriteComponents;
    // 텍스처 load를 위한 Map
    std::unordered_map<std::string, SDL_Texture*> mTextures;
};

