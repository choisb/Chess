#include "Actor.h"
#include "Game.h"
Actor::Actor(Game& game) 
    : mGame{ game }
    , mState {EActive}
    , mLocation {Vector2::Zero}
    , mScale {0.f}
    , mRotation {0.f}
{
    SDL_Log("Actor::Actor()");

}
Actor::~Actor()
{
    SDL_Log("Actor::~Actor()");    
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
    // TODO:: Update Components
}