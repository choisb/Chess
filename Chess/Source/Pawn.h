#pragma once
#include "Piece.h"

class Pawn : public Piece
{
public:
    Pawn(class Game& game, class GameManager& gameManager, Color inColor, Coordinates2 inPosition, size_t size = 128);
    ~Pawn();

    void UpdateNextPosition() override {}

private:
};


