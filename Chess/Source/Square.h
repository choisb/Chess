#pragma once
#include "Core/Actor.h"
#include <memory>

class SpriteComponent;
class Piece;

class Square : public Actor
{
public:
    Square(class Game& game, Coordinates2 inCoordinates2, size_t size = 128);
    void Occupied(const std::shared_ptr<Piece>& piece);
    PieceType GetTypeOfPiece() const;
    
private:
    Coordinates2 mPosition;

    // Square의 sprite
    std::weak_ptr<SpriteComponent> mSpriteComponent;
    // Square에 놓여져 있는 기물
    std::weak_ptr<Piece> mPiece;
};


