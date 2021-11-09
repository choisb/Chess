#include "Piece.h"
#include "GameManager.h"
#include "Square.h"
#include "Core/Renderer.h"
#include "Core/SpriteComponent.h"
#include "Core/Game.h"

Piece::Piece(Game& game, GameManager& gameManager, PieceType type, Color inColor, Coordinates2 inPosition, size_t size)
    :Actor(game)
    ,mGameManager(gameManager)
    ,mType(type)
    ,mColor(inColor)
    ,mCurrentPosition(std::move(inPosition))
    ,mSize(size)
{
    mSpriteComponent = CreateComponent<SpriteComponent>(*this, 80);
}
Piece::~Piece()
{

}
void Piece::Initialize()
{
    // ��������Ʈ ũ�� ����
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        const float textureSize = static_cast<float>(sc->GetTexHeight());
        const float newScale = static_cast<float>(mSize) / textureSize;
        SetActorScale(newScale);
    }

    // �⹰ ��ġ �ʱ�ȭ (Next position ���� ����. ��� �⹰ ���� �� GameManager���� ȣ����)
    MovePieceTo(mCurrentPosition, false);
}
void Piece::MovePieceTo(Coordinates2 inPosition, bool bIsUpdateAllNextPosition /*= true*/)
{
    // TODO: ���� ĭ UnOccupied �ؾ���.

    // �ش� ��ġ�� �̵�
    SetActorLocation(mGameManager.GetLocationOf(mCurrentPosition));
    auto square = mGameManager.GetSquare(mCurrentPosition).lock();
    if (square)
    {
        // �ش� ��ġ ĭ�� ǥ��
        square->Occupied(std::static_pointer_cast<Piece>(shared_from_this()));
    }
    // UpdateNextPosition�� true�� ��쿡�� NextPosition�� ���Ӱ� �����Ѵ�.
    // GameManager �ʱ�ȭ�� bIsUpdateAllNextPosition�� false
    // �Ϲ����� ���(�ϳ��� �⹰�� ������ ���) bIsUpdateAllNextPosition�� true
    if (bIsUpdateAllNextPosition) mGameManager.UpdateAllNextPositionOfPiece();
}
void Piece::BeAttacked()
{
    SetState(State::EDead);
    mGameManager.RemovePiece(std::static_pointer_cast<Piece>(shared_from_this()), mColor);
}


// Pawn ����
Pawn::Pawn(Game& game, GameManager& gameManager, Color inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Pawn, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == Color::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_pawn_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_pawn_png_shadow_128px.png"));
    }

}

Pawn::~Pawn()
{

}

// Knight ����
Knight::Knight(Game& game, GameManager& gameManager, Color inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Knight, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == Color::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_knight_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_knight_png_shadow_128px.png"));
    }

}

Knight::~Knight()
{

}

// Bishop ����
Bishop::Bishop(Game& game, GameManager& gameManager, Color inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Bishop, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == Color::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_Bishop_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_Bishop_png_shadow_128px.png"));
    }

}

Bishop::~Bishop()
{

}

// Rook ����
Rook::Rook(Game& game, GameManager& gameManager, Color inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Rook, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == Color::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_Rook_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_Rook_png_shadow_128px.png"));
    }

}

Rook::~Rook()
{

}

// Queen ����
Queen::Queen(Game& game, GameManager& gameManager, Color inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Queen, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == Color::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_Queen_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_Queen_png_shadow_128px.png"));
    }

}

Queen::~Queen()
{

}

// King ����
King::King(Game& game, GameManager& gameManager, Color inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::King, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == Color::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_King_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_King_png_shadow_128px.png"));
    }

}

King::~King()
{

}