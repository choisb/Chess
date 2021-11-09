#include "Piece.h"
#include "GameManager.h"
#include "Square.h"
#include "Core/Renderer.h"
#include "Core/SpriteComponent.h"
#include "Core/Game.h"

Piece::Piece(Game& game, GameManager& gameManager, PieceType type, PieceColor inColor, Coordinates2 inPosition, size_t size)
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
    // 스프라이트 크기 설정
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        const float textureSize = static_cast<float>(sc->GetTexHeight());
        const float newScale = static_cast<float>(mSize) / textureSize;
        SetActorScale(newScale);
    }

    // 기물 위치 초기화 (Next position 갱신 안함. 모든 기물 생성 후 GameManager에서 호출함)
    MovePieceTo(mCurrentPosition, false);
}
void Piece::MovePieceTo(const Coordinates2& inPosition, bool bIsUpdateAllNextPosition /*= true*/)
{
    // 현재 칸 UnOccupied
    auto square = mGameManager.GetSquare(mCurrentPosition);
    square->UnOccupied();

    // 해당 위치로 이동
    mCurrentPosition = inPosition;
    SetActorLocation(mGameManager.GetActorLocationOf(mCurrentPosition));
    square = mGameManager.GetSquare(mCurrentPosition);

    // 해당 위치 칸에 표시
    square->Occupied(std::static_pointer_cast<Piece>(shared_from_this()));
    
    // UpdateNextPosition이 true일 경우에만 NextPosition을 새롭게 갱신한다.
    // GameManager 생성시 bIsUpdateAllNextPosition는 false로 설정.
    // GameManager 생성 이후 GameManager::Initialize() 함수에서 별도로 UpdateAllNextPositionOfPiece() 함수 호출
    // 일반적인 경우(하나의 기물이 움직일 경우) bIsUpdateAllNextPosition는 true
    if (bIsUpdateAllNextPosition) mGameManager.UpdateAllNextPositionOfPiece();
}
void Piece::BeAttacked()
{
    SetState(State::EDead);
    mGameManager.RemovePiece(std::static_pointer_cast<Piece>(shared_from_this()), mColor);
}


// Pawn 구현
Pawn::Pawn(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Pawn, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == PieceColor::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_pawn_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_pawn_png_shadow_128px.png"));
    }

}

Pawn::~Pawn()
{

}

// Knight 구현
Knight::Knight(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Knight, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == PieceColor::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_knight_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_knight_png_shadow_128px.png"));
    }

}

Knight::~Knight()
{

}

// Bishop 구현
Bishop::Bishop(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Bishop, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == PieceColor::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_Bishop_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_Bishop_png_shadow_128px.png"));
    }

}

Bishop::~Bishop()
{

}

// Rook 구현
Rook::Rook(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Rook, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == PieceColor::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_Rook_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_Rook_png_shadow_128px.png"));
    }

}

Rook::~Rook()
{

}

// Queen 구현
Queen::Queen(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Queen, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == PieceColor::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_Queen_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_Queen_png_shadow_128px.png"));
    }

}

Queen::~Queen()
{

}

// King 구현
King::King(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::King, inColor, inPosition, size)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == PieceColor::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_King_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_King_png_shadow_128px.png"));
    }

}

King::~King()
{

}