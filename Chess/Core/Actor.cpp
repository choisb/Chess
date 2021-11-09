#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "SpriteComponent.h"

Actor::Actor(Game& game) 
    : enable_shared_from_this<Actor>()
    , mGame{ game }
    , mState {State::EActive}
    , mLocation {Vector2::Zero}
    , mScale {1.f}
    , mRotation {0.f}
{
    mGame.IncreaseCreatedActorCount();
    //SDL_Log("Actor::Actor()");
}
Actor::~Actor()
{
    mGame.IncreaseDestroyedActorCount();
    //SDL_Log("Actor::~Actor()");    
    mComponents.clear();
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
    // Component의 우선순위대로 Update 함수가 호출됨
    for (auto component : mComponents)
    {
        component->Update(deltaTime);
    }
}

// Component의 우선순위대로 벡터에 삽입. (시간복잡도: O(n))
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
