#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include <SDL/SDL.h>

SpriteComponent::SpriteComponent(Actor& owner, int drawOrder)
    : Component(owner)
    , enable_shared_from_this<SpriteComponent>()
    , mTexture(nullptr)
    , mDrawOrder(drawOrder)
    , mTexWidth(0)
    , mTexHeight(0)
{
    // Sprite component는 Actor의 mCmoponents와 Game의 mSprites 두곳에 모두 저장됨
    // Actor에서 생성할때 mComponents에 추가되고
    // 생성자에서 mSprites에 추가함
    SDL_Log("SpriteComponent()");
}
SpriteComponent::~SpriteComponent()
{
    // Game의 mSpriteComponents와 Actor의 mComponents가 모두 clear될때 자동으로 소멸됨

    SDL_Log("~SpriteComponent()");
}
void SpriteComponent::Initialize()
{
    mOwner.GetGame().AddSpriteToArray(shared_from_this());
}
void SpriteComponent::SetTexture(SDL_Texture* texture)
{
    mTexture = texture;
    // 텍스처의 너비와 높이를 알기 위해서 SDL_QuertTexture() 함수 사용
    SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
void SpriteComponent::Draw(SDL_Renderer* renderer)
{
    if (mTexture)
    {
        SDL_Rect r;
        // mOwner의 스케일 값으로 너비/높이 설정
        r.w = static_cast<int>(mTexWidth * mOwner.GetScale());
        r.h = static_cast<int>(mTexHeight * mOwner.GetScale());
        // 소유자의 위치를 중심으로 사각형 배치
        r.x = static_cast<int>(mOwner.GetLocation().x - r.w / 2);
        r.y = static_cast<int>(mOwner.GetLocation().y - r.h / 2);

        // 스프라이트를 그린다.
        SDL_RenderCopyEx(renderer,
            mTexture,       // 그릴 텍스처
            nullptr,        // 그릴 텍스처의 영역, nullptr이면 전체 영역
            &r,             // 그려질 대상의 사각형 영역
            -Math::ToDegrees(mOwner.GetRotation()),    // 라디안을 각도로 변환
            nullptr,        // 회전 기준점
            SDL_FLIP_NONE); // 플립 행동
    }
}