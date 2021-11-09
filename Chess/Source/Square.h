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

    // Square�� sprite
    std::weak_ptr<SpriteComponent> mSpriteComponent;
    // Square�� ������ �ִ� �⹰
    std::weak_ptr<Piece> mPiece;
};


