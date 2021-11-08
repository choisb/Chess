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
    // Sprite component�� Actor�� mCmoponents�� Game�� mSprites �ΰ��� ��� �����
    // Actor���� �����Ҷ� mComponents�� �߰��ǰ�
    // �����ڿ��� mSprites�� �߰���
    SDL_Log("SpriteComponent()");
}
SpriteComponent::~SpriteComponent()
{
    // Game�� mSpriteComponents�� Actor�� mComponents�� ��� clear�ɶ� �ڵ����� �Ҹ��

    SDL_Log("~SpriteComponent()");
}
void SpriteComponent::Initialize()
{
    mOwner.GetGame().AddSpriteToArray(shared_from_this());
}
void SpriteComponent::SetTexture(SDL_Texture* texture)
{
    mTexture = texture;
    // �ؽ�ó�� �ʺ�� ���̸� �˱� ���ؼ� SDL_QuertTexture() �Լ� ���
    SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
void SpriteComponent::Draw(SDL_Renderer* renderer)
{
    if (mTexture)
    {
        SDL_Rect r;
        // mOwner�� ������ ������ �ʺ�/���� ����
        r.w = static_cast<int>(mTexWidth * mOwner.GetActorScale());
        r.h = static_cast<int>(mTexHeight * mOwner.GetActorScale());
        // �������� ��ġ�� �߽����� �簢�� ��ġ
        r.x = static_cast<int>(mOwner.GetActorLocation().x - r.w / 2);
        r.y = static_cast<int>(mOwner.GetActorLocation().y - r.h / 2);

        // ��������Ʈ�� �׸���.
        SDL_RenderCopyEx(renderer,
            mTexture,       // �׸� �ؽ�ó
            nullptr,        // �׸� �ؽ�ó�� ����, nullptr�̸� ��ü ����
            &r,             // �׷��� ����� �簢�� ����
            -Math::ToDegrees(mOwner.GetActorRotation()),    // ������ ������ ��ȯ
            nullptr,        // ȸ�� ������
            SDL_FLIP_NONE); // �ø� �ൿ
    }
}