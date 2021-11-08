#pragma once
#include "Core/Actor.h"
#include <memory>
class Square : public Actor
{
public:
    Square(class Game& game, Coordinates2 inCoordinates2, size_t size = 128);
    
private:
    Coordinates2 mPosition;

    // 0: empty, 그 외: 기물 번호
    int mStateOfOccupy;
    // Square의 sprite
    std::weak_ptr<class SpriteComponent> mSpriteComponent;
    // Square에 놓여져 있는 기물
    std::weak_ptr<class Pice> mPice;
};


