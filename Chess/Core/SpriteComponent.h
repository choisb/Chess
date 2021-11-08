#pragma once
#include "Component.h"
#include "SDL/SDL.h"
#include <memory>
class SpriteComponent : public Component, public std::enable_shared_from_this<SpriteComponent>
{
public:
    // (�׸��� �������� ���� ���� �� ���ʿ� �׸���.)
    SpriteComponent(class Actor& owner, int drawOrder = 100);
    ~SpriteComponent();
    void Initialize() override;
    void Draw(SDL_Renderer* renderer);
    void SetTexture(SDL_Texture* texture);
    void SetDrawOrder(int drawOrder) { mDrawOrder = drawOrder; }

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }

protected:
    // �׸� �ؽ�ó
    SDL_Texture* mTexture;
    // ȭ�� �˰��򿡼� ����ϴ� �׸��� ����
    int mDrawOrder;
    // �ؽ�ó�� �ʺ� / ����
    int mTexWidth;
    int mTexHeight;
};