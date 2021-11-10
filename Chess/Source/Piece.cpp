#include "Piece.h"
#include "GameManager.h"
#include "Square.h"
#include "Core/Renderer.h"
#include "Core/SpriteComponent.h"
#include "Core/Game.h"
#include "Core/Math.h"
Piece::Piece(Game& game, GameManager& gameManager, PieceType type, PieceColor inColor, Coordinates2 inPosition, size_t size)
    :Actor(game)
    ,mGameManager(gameManager)
    ,mType(type)
    ,mColor(inColor)
    ,mCurrentPosition(std::move(inPosition))
    ,mSize(size)
{
    mSpriteComponent = CreateComponent<SpriteComponent>(*this, 70);
    mEnemyColor = PieceColor::Black;
    if (mColor == PieceColor::Black)
        mEnemyColor = PieceColor::White;
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

    // 기물 위치 초기화 
    SetActorLocation(mGameManager.GetActorLocationOf(mCurrentPosition));

    // 해당 칸에 기물 점유 표시
    auto square = mGameManager.GetSquare(mCurrentPosition);
    square->Occupied(std::static_pointer_cast<Piece>(shared_from_this()));
}
void Piece::MovePieceTo(const Coordinates2& nextPosition)
{
    // 현재 칸 UnOccupied
    auto square = mGameManager.GetSquare(mCurrentPosition);
    square->UnOccupied();
    square->Unselected();

    // 이동하기 전 이동 후보지로 마킹되어있던 Square 모두 후보설정 해제
    for (const auto& next : mNextPosition)
    {
        mGameManager.GetSquare(next)->CancelCandidate();
    }

    // 해당 위치로 이동
    mCurrentPosition = nextPosition;
    SetActorLocation(mGameManager.GetActorLocationOf(mCurrentPosition));

    // 해당 위치 칸에 표시
    square = mGameManager.GetSquare(mCurrentPosition);
    square->Occupied(std::static_pointer_cast<Piece>(shared_from_this()));
}
void Piece::StartTurn()
{
    //기물의 next position을 갱신
    UpdateNextPosition();
}
void Piece::BeAttacked()
{
    SetState(State::EDead);
    mGameManager.RemovePiece(std::static_pointer_cast<Piece>(shared_from_this()), mColor);
}

void Piece::Selected()
{
    // 현재 위치 Square 선택
    mGameManager.GetSquare(mCurrentPosition)->Selected();
    // 후보 위치 Square 모두 후보지로 선택
    for (auto& next : mNextPosition)
    {
        mGameManager.GetSquare(next)->BeCandidate();
    }
}

void Piece::Unselected()
{
    // 현재 위치 Square 선택 해제
    mGameManager.GetSquare(mCurrentPosition)->Unselected();
    // 후보 위치 Square 모두 후보지 해제
    for (auto& next : mNextPosition)
    {
        mGameManager.GetSquare(next)->CancelCandidate();
    }
}


// Pawn 구현
Pawn::Pawn(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Pawn, inColor, inPosition, size)
    , mMoveDirection(0,1) // 블랙인 경우 (0,1)
    , mbFirstMove(false)
    , mbMoveTwoSquare(false)
    , mbCanEnpassant(false)
{
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        if (inColor == PieceColor::Black)
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/b_pawn_png_shadow_128px.png"));
        else
            sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/w_pawn_png_shadow_128px.png"));
    }
 
    // white인 경우 (0, -1)로 변경
    if (inColor == PieceColor::White)
        mMoveDirection.y = -1;
}

Pawn::~Pawn()
{

}
void Pawn::MovePieceTo(const Coordinates2& nextPosition) /*override*/
{
    if (Math::Abs(nextPosition.y - mCurrentPosition.y) == 2)
        mbMoveTwoSquare = true;
    Coordinates2 side(nextPosition.x, mCurrentPosition.y);

    // 앙파상 판정, 앙파상이 맞다면 앙파상 실행
    if (IsThisMoveEnpassant(side))
    {
        DoEnpassant(side);
    }

    // 이동
    Piece::MovePieceTo(nextPosition);

    // 첫번째 움직임 설정
    mbFirstMove = true;
    //TODO: 폰 프로모션 기능 추가, UI 기능 구현 후 적용 
}
bool Pawn::IsThisMoveEnpassant(const Coordinates2& side)
{
    auto sidePiece = mGameManager.GetSquare(side)->GetPiece().lock();
    // 대각선으로 이동하고, 이동하는 방향 옆에 기물이 존재하며, 그 기물이 적군 이며, 적군 기물이 폰인경우
    if (side.x != mCurrentPosition.x && sidePiece && sidePiece->GetColor() != mColor && sidePiece->GetType() == PieceType::Pawn)
    {
        auto enemyPawn = std::static_pointer_cast<Pawn>(sidePiece);
        // 적 Pawn이 이전 턴에 두 칸이상 움직였다면 앙파상이기 때문에 공격
        if (enemyPawn->IsMoveTwoSquare())
        {
            return true;
        }
    }
    return false;
}
void Pawn::DoEnpassant(const Coordinates2& side)
{
    auto enemyPawn = std::static_pointer_cast<Pawn>(mGameManager.GetSquare(side)->GetPiece().lock());
    enemyPawn->BeAttacked();
}
void Pawn::StartTurn()
{
    Piece::StartTurn();
    mbMoveTwoSquare = false;
}
void Pawn::UpdateNextPosition()
{
    mNextPosition.clear();
    UpdateForwardDirection();
    UpdateDiagonalDirection(Coordinates2(1, 0)); // 우측 대각선 방향 탐색
    UpdateDiagonalDirection(Coordinates2(-1, 0)); // 좌측 대각선 방향 탐색
    UpdateEnpassantDirection(Coordinates2(1,0)); // 우측방향 앙파상
    UpdateEnpassantDirection(Coordinates2(-1,0)); // 좌측방향 앙파상
}

void Pawn::UpdateForwardDirection()
{
    // 전방이 비었다면
    Coordinates2 forward = mCurrentPosition + mMoveDirection;

    if (GameManager::ValidIndex(forward) && mGameManager.GetSquare(forward)->IsEmpty())
    {
        mNextPosition.push_back(forward);
        if (mbFirstMove == false) // 한번도 움직인적 없다면 두칸 움직일 수 있다.
        {
            forward += mMoveDirection;
            if (mGameManager.GetSquare(forward)->IsEmpty())
                mNextPosition.push_back(forward);
        }
    }
}

void Pawn::UpdateDiagonalDirection(const Coordinates2& sideDirection)
{
    const Coordinates2 diagonal = mCurrentPosition + mMoveDirection - sideDirection;
    // 대각선 방향이 범위를 벗어났다면 탐색 종료
    if (!GameManager::ValidIndex(diagonal)) return;
    //대각선에 적군이 있다면
    if (mGameManager.GetSquare(diagonal)->GetColor() == mEnemyColor)
    {
        mNextPosition.push_back(diagonal);
    }
}

void Pawn::UpdateEnpassantDirection(const Coordinates2& sideDirection)
{
    const Coordinates2 side = mCurrentPosition + sideDirection;
    // 보드 범위를 벗어날 경우 종료
    if (!GameManager::ValidIndex(side)) return;

    auto sidePiece = mGameManager.GetSquare(side)->GetPiece().lock();
    // direction방향의 옆쪽 piece가 존재하고, piece 타입이 pawn 이라면
    if (sidePiece && sidePiece->GetType() == PieceType::Pawn)
    {
        auto pawn = std::static_pointer_cast<Pawn>(sidePiece);
        if (pawn->IsMoveTwoSquare()) // 앙파상 가능
            mNextPosition.push_back(side + mMoveDirection);
    }
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