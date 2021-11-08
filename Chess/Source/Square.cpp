#include "Square.h"
#include "Core/Game.h"
#include "Core/SpriteComponent.h"
Square::Square(Game& game, Coordinates2 inCoordinates2, size_t size /* = 128*/)
    : Actor(game)
    , mPosition(std::move(inCoordinates2))
    , mStateOfOccupy(0)
{
    // Sprite 그리는 우선순위 50
    if (size != 0) mSpriteComponent = CreateComponent<SpriteComponent>(*this,50);

    auto sc = mSpriteComponent.lock();
    if (sc)
    {
        if ((mPosition.x + mPosition.y) % 2 == 0)
            sc->SetTexture(GetGame().GetTexture("../Chess/Assets/Imgs/square brown light_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetTexture("../Chess/Assets/Imgs/square brown dark_png_shadow_128px.png"));

        const float textureSize = static_cast<float>(sc->GetTexHeight());
        const float newScale = static_cast<float>(size) / textureSize;
        SetActorScale(newScale);
        const float mRowOffset = mPosition.y * size + (textureSize / 2) * newScale;
        const float mColOffset = mPosition.x * size + (textureSize / 2) * newScale;

        SetActorLocation(Vector2(mColOffset, mRowOffset));
    }
}
