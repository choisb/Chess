#include "Actor.h"
#include "Game.h"
#include "Component.h"
Actor::Actor(Game& game) 
    : mGame{ game }
    , mState {EActive}
    , mLocation {Vector2::Zero}
    , mScale {0.f}
    , mRotation {0.f}
{
    SDL_Log("Actor::Actor()");
    AddComponent();
    AddComponent();

}
Actor::~Actor()
{
    SDL_Log("Actor::~Actor()");    
    mComponents.clear();
}
void Actor::Update(float deltaTime)
{
    UpdateActor(deltaTime);
    UpdateComponents(deltaTime);
}
void Actor::UpdateActor(float deltaTime)
{
    if (mState == EPaused) return;

}
void Actor::UpdateComponents(float deltaTime)
{
    // Component의 우선순위대로 Update 함수가 호출됨
    for (auto component : mComponents)
    {
        component->Update(deltaTime);
    }
}
std::weak_ptr<Component> Actor::AddComponent()
{
    std::weak_ptr<Component> compWeakPtr;
    std::shared_ptr<Component> compSharedPtr = std::make_shared<Component>(*this);
    AddComponentToArray(compSharedPtr);
    compWeakPtr = compSharedPtr;
    return compWeakPtr;
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
