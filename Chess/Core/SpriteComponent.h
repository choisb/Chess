#pragma once
#include "Component.h"
#include "SDL/SDL.h"
#include <memory>
class SpriteComponent : public Component
{
public:
    // (�׸��� �������� ���� ���� �� ���ʿ� �׸���.)
    SpriteComponent(class Actor& owner, int drawOrder = 100);
    ~SpriteComponent();
    void Initialize() override;
    void Shutdown() override;
    void Update(float deltaTime) override {}
    void Draw(SDL_Renderer* renderer);
    
    // Setter & Getter
    void SetTexture(SDL_Texture* texture);
    void SetDrawOrder(int drawOrder) { mDrawOrder = drawOrder; }

    int GetDrawOrder() const { return mDrawOrder; }
    int GetTexHeight() const { return mTexHeight; }
    int GetTexWidth() const { return mTexWidth; }

    // sprite Ȱ��ȭ / �� Ȱ��ȭ
    void SetDisable() { mbIsDraw = false; }
    void SetAble() { mbIsDraw = true; }

protected:
    // �׸� �ؽ�ó
    SDL_Texture* mTexture;
    // ȭ�� �˰��򿡼� ����ϴ� �׸��� ����
    int mDrawOrder;
    // �ؽ�ó�� �ʺ� / ����
    int mTexWidth;
    int mTexHeight;

    // false�ϰ�� �׷����� �ʴ´�.
    bool mbIsDraw;
};