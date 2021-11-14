#pragma once
#include "Component.h"
#include "SDL/SDL.h"
#include <memory>
class SpriteComponent : public Component
{
public:
    // (그리기 순서값이 낮을 수록 더 뒤쪽에 그린다.)
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

    // sprite 활성화 / 비 활성화
    void SetDisable() { mbIsDraw = false; }
    void SetAble() { mbIsDraw = true; }

protected:
    // 그릴 텍스처
    SDL_Texture* mTexture;
    // 화가 알고리즘에서 사용하는 그리기 순서
    int mDrawOrder;
    // 텍스처의 너비 / 높이
    int mTexWidth;
    int mTexHeight;

    // false일경우 그려지지 않는다.
    bool mbIsDraw;
};