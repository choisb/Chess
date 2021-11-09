#include "Piece.h"
#include "PieceTypes.h"
#include "Square.h"
#include "GameManager.h"
#include "Core/Game.h"
#include "Core/SpriteComponent.h"
#include "Core/Renderer.h"

Square::Square(Game& game, GameManager& gameManager, Coordinates2 inCoordinates2, size_t size /* = 128*/)
    : Actor(game)
    , mGameManager(gameManager)
    , mPosition(std::move(inCoordinates2))
    , mSize(size)
{
    // size�� 0���� �۰ų� ���� ��� defalut ������ �ʱ�ȭ
    if (size <= 0) mSize = 128;

    // NormalSprite �׸��� �켱���� 50
    mNormalSprite = CreateComponent<SpriteComponent>(*this,50);

    auto sc = mNormalSprite.lock();
    if ((mPosition.x + mPosition.y) % 2 == 0)
        sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/square brown light_png_shadow_128px.png"));
    else
        sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/square brown dark_png_shadow_128px.png"));

    // Highlight Sprite �׸��� �켱������ 60
    mHighlightSprite = CreateComponent<SpriteComponent>(*this, 60);
    sc = mHighlightSprite.lock();
    sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/selected_square_128px.png"));
    sc->SetDisable();
}
void Square::Initialize()
{
    auto sc = mNormalSprite.lock();
 
    const float textureSize = static_cast<float>(sc->GetTexHeight());
    const float newScale = static_cast<float>(mSize) / textureSize;
    SetActorScale(newScale);
    const float mRowOffset = mPosition.y * mSize + (textureSize / 2) * newScale;
    const float mColOffset = mPosition.x * mSize + (textureSize / 2) * newScale;
    SetActorLocation(Vector2(mColOffset, mRowOffset));
}

PieceType Square::GetTypeOfPiece() const
{
    auto currentPiece = mPiece.lock();
    // ���� �⹰�� �ִ� ���
    if (currentPiece)
    {
        return currentPiece->GetType();
    }
    return PieceType::Default_MAX;
}

void Square::Occupied(const std::shared_ptr<Piece>& piece)
{
    auto currentPiece = mPiece.lock();
    // ���� �⹰�� �ִ� ���
    if (currentPiece)
    {
        currentPiece->BeAttacked();
    }
    mPiece = piece; 
}
void Square::Selected()
{
    mGameManager.SetSelectedSquare(std::move(std::static_pointer_cast<Square>(shared_from_this())));
    mHighlightSprite.lock()->SetAble();
}
void Square::UnSelected()
{
    mGameManager.ResetSelectedSquare();
    mHighlightSprite.lock()->SetDisable();

}
