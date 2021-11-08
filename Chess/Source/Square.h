#pragma once
#include "Core/Actor.h"
#include <memory>
class Square : public Actor
{
public:
    Square(class Game& game, Coordinates2 inCoordinates2, size_t size = 128);
    
private:
    Coordinates2 mPosition;

    // 0: empty, �� ��: �⹰ ��ȣ
    int mStateOfOccupy;
    // Square�� sprite
    std::weak_ptr<class SpriteComponent> mSpriteComponent;
    // Square�� ������ �ִ� �⹰
    std::weak_ptr<class Pice> mPice;
};


