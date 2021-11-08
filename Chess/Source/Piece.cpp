#include "Piece.h"
#include "GameManager.h"
#include "Core/SpriteComponent.h"
Piece::Piece(Game& game, GameManager& gameManager, PiceType type, Color inColor, Coordinates2 inPosition)
    :Actor(game)
    ,mType(type)
    ,mColor(inColor)
    ,mCurrentPosition(std::move(inPosition))
    ,mGameManager(gameManager)
{
    mSpriteCompoent = CreateComponent<SpriteComponent>(*this, 80);
}
Piece::~Piece()
{

}