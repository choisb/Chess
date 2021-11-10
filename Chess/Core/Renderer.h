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
    // �ʱ�ȭ �Լ�
    bool Initialize();
    // ���� �Լ�. SDL ���ҽ� ����
    void Shutdown();
    // ��� �Լ�
    void Draw();
    // Texture �ε� �Լ�
    SDL_Texture* GetTexture(const std::string& fileName);
    // ���ο� Sprite�� �迭�� �߰��ϴ� �Լ�
    void AddSpriteToArray(const std::shared_ptr<SpriteComponent>& spriteComponent);
    // �迭���� Sprite�� �����ϴ� �Լ�
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

    // Sprite ���� �����͸� ��� �ִ� �迭
    std::vector<std::shared_ptr<SpriteComponent>> mSpriteComponents;
    // �ؽ�ó load�� ���� Map
    std::unordered_map<std::string, SDL_Texture*> mTextures;
};

