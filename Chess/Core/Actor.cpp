#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "SpriteComponent.h"
#include "Debugging.h"

Actor::Actor(Game& game) 
    : enable_shared_from_this<Actor>()
    , mGame{ game }
    , mState {State::EActive}
    , mLocation {Vector2::Zero}
    , mScale {1.f}
    , mRotation {0.f}
{
    CONSTRUCT_LOG();
    mGame.IncreaseCreatedActorCount();
}
Actor::~Actor()
{
    DESTRUCTOR_LOG();
    mGame.IncreaseDestroyedActorCount();

    mComponents.clear();
}
void Actor::Shutdown()
{
    for (auto& component : mComponents)
    {
        component->Shutdown();
    }
}
void Actor::Update(float deltaTime)
{
    UpdateActor(deltaTime);
    UpdateComponents(deltaTime);
}
void Actor::UpdateActor(float deltaTime)
{
    if (mState == State::EPaused) return;

}
void Actor::UpdateComponents(float deltaTime)
{
    // Component�� �켱������� Update �Լ��� ȣ���
    for (auto component : mComponents)
    {
        component->Update(deltaTime);
    }
}

// Component�� �켱������� ���Ϳ� ����. (�ð����⵵: O(n))
void Actor::AddComponentToArray(std::shared_ptr<Component> component)
{
    auto iter = mComponents.begin();
    for (; iter != mComponents.end(); iter++)
    {
        if ((*iter)->GetUpdateOrder() > component->GetUpdateOrder())
            break;
    }
    mComponents.insert(iter, component);
}
