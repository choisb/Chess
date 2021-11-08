#pragma once
#include "Core/Actor.h"
#include "Core/Math.h"
#include <memory>
#include <vector>
enum class Color : uint8_t {
    Withe, Black, Default_MAX
};
enum class PiceType : uint8_t {
    Pawn, Knight, Bishop, Rook, Queen, King , Default_MAX
};
class Piece : public Actor
{
public:
    Piece(class Game& game, class GameManager& gameManager, PiceType type, Color color, Coordinates2 position);
    ~Piece();

    void MovePiceTo(Coordinates2 nextCoordinates);
    virtual void UpdateNextPosition() = 0;

protected:
    std::weak_ptr<class SpriteComponent> mSpriteCompoent;
    Coordinates2 mCurrentPosition;
    std::vector<Coordinates2> mNextPosition;
    PiceType mType;
    Color mColor;
    GameManager& mGameManager;

};


