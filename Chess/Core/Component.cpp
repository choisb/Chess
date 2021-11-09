#include "Component.h"
#include "Actor.h"
#include "SDL/SDL.h"

Component::Component(Actor& owner, int updateOrder)
    : enable_shared_from_this<Component>()
    , mOwner(owner)
    , mUpdateOrder(updateOrder)
{
    //SDL_Log("Component()");
}
Component::~Component()
{
    //SDL_Log("~Component()");

}
void Component::Initialize()
{

}
void Component::Update(float deltaTime)
{

}
