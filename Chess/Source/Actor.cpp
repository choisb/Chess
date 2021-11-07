#include "Actor.h"
#include "Game.h"
Actor::Actor(Game& game) 
    : mGame{ game }
    , mState {EActive}
{
    SDL_Log("Actor::Actor()");

    mGame.AddActor(this);
}
Actor::~Actor()
{
    SDL_Log("Actor::~Actor()");

    mGame.RemoveActor(this);
}
void Actor::Update(float deltaTime)
{
    UpdateActor(deltaTime);
    UpdateComponents(deltaTime);
}
void Actor::UpdateActor(float deltaTime)
{
    if (mState == EPaused) return;
    if (mState == EDead)
    {
        mGame.RemoveActor(this);
    }
    mState = EDead;
}
void Actor::UpdateComponents(float deltaTime)
{
    // TODO:: Update Components
}