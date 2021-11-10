#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include <SDL/SDL.h>

SpriteComponent::SpriteComponent(Actor& owner, int drawOrder)
    : Component(owner)
    , mTexture(nullptr)
    , mDrawOrder(drawOrder)
    , mTexWidth(0)
    , mTexHeight(0)
    , mbIsDraw(true)
{
    // Sprite component�� Actor�� mCmoponents�� Renderer�� mSprites �ΰ��� ��� �����
    // Actor���� �����Ҷ� mComponents�� �߰��ǰ�
    // �����ڿ��� mSprites�� �߰���
    CONSTRUCT_LOG();
}
SpriteComponent::~SpriteComponent()
{
    // Game�� mSpriteComponents�� Actor�� mComponents�� ��� clear�ɶ� �ڵ����� �Ҹ��
    DESTRUCTOR_LOG();
}
void SpriteComponent::Initialize()
{
    Component::Initialize();
    mOwner.GetGame().GetRenderer()->AddSpriteToArray(std::static_pointer_cast<SpriteComponent>(shared_from_this()));
}
void SpriteComponent::Shutdown()
{
    Component::Shutdown();
    mOwner.GetGame().GetRenderer()->RemoveSpriteFromArray(std::static_pointer_cast<SpriteComponent>(shared_from_this()));
}
void SpriteComponent::SetTexture(SDL_Texture* texture)
{
    mTexture = texture;
    // �ؽ�ó�� �ʺ�� ���̸� �˱� ���ؼ� SDL_QuertTexture() �Լ� ���
    SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
void SpriteComponent::Draw(SDL_Renderer* renderer)
{
    if (mbIsDraw == false) return; // ��Ȱ��ȭ�� ��� �׸��� ����.
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