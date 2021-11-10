#include "Component.h"
#include "Actor.h"
#include "Game.h"
#include "SDL/SDL.h"

Component::Component(Actor& owner, int updateOrder)
    : enable_shared_from_this<Component>()
    , mOwner(owner)
    , mUpdateOrder(updateOrder)
{
    CONSTRUCT_LOG();
    mOwner.GetGame().IncreaseCreatedComponentCount();
    //SDL_Log("Component()");
}
Component::~Component()
{
    DESTRUCTOR_LOG();
    mOwner.GetGame().IncreaseDestroyedComponentCount();
    //SDL_Log("~Component()");
}
void Component::Initialize()
{

}
void Component::Update(float deltaTime)
{

}
