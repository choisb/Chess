#include "Pawn.h"
#include "GameManager.h"
#include "Core/SpriteComponent.h"
#include "Core/Game.h"
Pawn::Pawn(Game& game, GameManager& gameManager, Color inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PiceType::Pawn, inColor, inPosition)
{
    auto sc = mSpriteCompoent.lock();
    if (sc)
    {
        if(inColor == Color::Black)
            sc->SetTexture(GetGame().GetTexture("../Chess/Assets/Imgs/b_pawn_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetTexture("../Chess/Assets/Imgs/w_pawn_png_shadow_128px.png"));
    }

    const float textureSize = static_cast<float>(sc->GetTexHeight());
    const float newScale = static_cast<float>(size) / textureSize;
    SetActorScale(newScale);

    SetActorLocation(mGameManager.GetLocationOf(mCurrentPosition));
}

Pawn::~Pawn()
{

}