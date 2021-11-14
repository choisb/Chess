#include "Piece.h"
#include "GameManager.h"
#include "Square.h"
#include "Core/Renderer.h"
#include "Core/SpriteComponent.h"
#include "Core/Game.h"
#include "Core/Math.h"

const std::array<Coordinates2, 4> Piece::StraightDirections = { Coordinates2(1,0), Coordinates2(-1,0), Coordinates2(0,1), Coordinates2(0,-1) };
const std::array<Coordinates2, 4> Piece::DiagonalDirections = { Coordinates2(1,1), Coordinates2(-1,1), Coordinates2(1,-1), Coordinates2(-1,-1) };


Piece::Piece(Game& game, GameManager& gameManager, PieceType type, PieceColor inColor, Coordinates2 inPosition, size_t size)
    : Actor(game)
    , mGameManager(gameManager)
    , mType(type)
    , mColor(inColor)
    , mCurrentPosition(std::move(inPosition))
    , mSize(size)
    , mbFirstMove(true)

{
    mSpriteComponent = CreateComponent<SpriteComponent>(*this, 70);

    if (mColor == PieceColor::Black) mEnemyColor = PieceColor::White;
    else mEnemyColor = PieceColor::Black;
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
    for (const auto& candidate : mMoveLocations)
    {
        mGameManager.GetSquare(candidate)->CancelCandidate();
    }

    // 해당 위치로 이동
    mCurrentPosition = nextPosition;
    SetActorLocation(mGameManager.GetActorLocationOf(mCurrentPosition));

    // 이동하려는 square
    square = mGameManager.GetSquare(mCurrentPosition);
    // 이동하려는 square에 기물이 있는경우 기물 파괴
    if (auto currentPiece = square->GetPiece().lock())
    {
        currentPiece->BeDestroyed();
    }
    // 점유 표시
    square->Occupied(std::static_pointer_cast<Piece>(shared_from_this()));

    // 첫번째 움직임 설정
    mbFirstMove = false;
}

void Piece::BeDestroyed()
{
    ReleaseAttackedLocation();
    ReleaseMoveLocation();
    SetState(State::EDead);
    mGameManager.RemovePiece(std::static_pointer_cast<Piece>(shared_from_this()), mColor);
}

void Piece::Selected()
{
    // 현재 위치 Square 선택
    mGameManager.GetSquare(mCurrentPosition)->Selected();
    // 후보 위치 Square 모두 후보지로 선택
    for (auto& next : mMoveLocations)
    {
        mGameManager.GetSquare(next)->BeCandidate();
    }
}

void Piece::Unselected()
{
    // 현재 위치 Square 선택 해제
    mGameManager.GetSquare(mCurrentPosition)->Unselected();
    // 후보 위치 Square 모두 후보지 해제
    for (auto& next : mMoveLocations)
    {
        mGameManager.GetSquare(next)->CancelCandidate();
    }
}

void Piece::AddAttackLocation(const Coordinates2 & position)
{
    auto square = mGameManager.GetSquare(position);
    // 공격하는 지역이 적군의 킹이 있는 곳이라면 Check 발생
    if (square->GetColor() == mEnemyColor && square->GetTypeOfPiece() == PieceType::King)
        mGameManager.BeChecked(mEnemyColor);

    square->BeAttackedBy(mColor);
    mAttackedLocations.push_back(position);
}

void Piece::ReleaseAttackedLocation()
{
    for (auto& position : mAttackedLocations)
    {
        mGameManager.GetSquare(position)->ReleaseFromAttackBy(mColor);
    }
    mAttackedLocations.clear();
}

void Piece::AddMoveLocation(const Coordinates2 & position)
{
    // TODO: 이동 가능하다면 즉, 이동했을 때 체크가 발생하지 않는다면 이동
    mMoveLocations.push_back(position);
}

void Piece::ReleaseMoveLocation()
{
    mMoveLocations.clear();
}

void Piece::SearchInTheDirection(const Coordinates2 & direction)
{
    auto next = mCurrentPosition + direction;
    while (GameManager::ValidIndex(next))
    {
        PieceColor nextColor = mGameManager.GetSquare(next)->GetColor();
        if (nextColor == PieceColor::Default_MAX)  // 빈칸: 공격, 이동 가능 탐색 계속
        {
            // TODO: 이동했을 때 체크가 발생하는지 확인
            AddAttackLocation(next);
            AddMoveLocation(next);
            next += direction;
        }
        else if (nextColor == mEnemyColor) // 적군: 공격, 이동 가능, 탐색 종료
        {
            // TODO: 이동했을 때 체크가 발생하는지 확인
            AddAttackLocation(next);
            AddMoveLocation(next);
            break;
        }
        else // 아군: 공격만 가능
        {
            AddAttackLocation(next);
            break;
        }
    }
}

bool Piece::CanMoveTo(const Coordinates2 & position)
{
    return false;
}


// Pawn 구현
Pawn::Pawn(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Pawn, inColor, inPosition, size)
    , mMoveDirection(0,1) // 블랙인 경우 (0,1)
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

    // 앙파상 판정, 앙파상이 맞다면 앙파상 실행
    if (IsThisMovementEnpassant(nextPosition))
    {
        DoEnpassant(nextPosition);
    }

    // 이동
    Piece::MovePieceTo(nextPosition);

    //TODO: 폰 프로모션 기능 추가, UI 기능 구현 후 적용 
}
bool Pawn::IsThisMovementEnpassant(const Coordinates2& nextPosition)
{
    Coordinates2 side(nextPosition.x, mCurrentPosition.y);
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
void Pawn::DoEnpassant(const Coordinates2& nextPosition)
{
    Coordinates2 side(nextPosition.x, mCurrentPosition.y);
    auto enemyPawn = std::static_pointer_cast<Pawn>(mGameManager.GetSquare(side)->GetPiece().lock());
    enemyPawn->BeDestroyed();
}
void Pawn::StartTurn()
{
    Piece::StartTurn();
    mbMoveTwoSquare = false;
}
void Pawn::SearchAttackAndMoveLocation()
{
    SearchForwardDirection();
    SearchDiagonalDirection(Coordinates2(1, mMoveDirection.y)); // 우측 대각선 방향 탐색
    SearchDiagonalDirection(Coordinates2(-1, mMoveDirection.y)); // 좌측 대각선 방향 탐색
    SearchEnpassantDirection(Coordinates2(1, mMoveDirection.y)); // 우측방향 앙파상
    SearchEnpassantDirection(Coordinates2(-1, mMoveDirection.y)); // 좌측방향 앙파상
}



void Pawn::SearchForwardDirection()
{
    // TODO: 이동했을 때 체크가 발생하는지 확인
    // 전방이 비었다면
    Coordinates2 forwardMovement = mCurrentPosition + mMoveDirection;

    if (GameManager::ValidIndex(forwardMovement) && mGameManager.GetSquare(forwardMovement)->IsEmpty())
    {
        AddMoveLocation(forwardMovement); // 공격은 할 수 없지만 이동은 할 수 있기 때문에 Move 배열에 추가
        if (mbFirstMove == true) // 한번도 움직인적이 없다면 두칸 움직일 수 있다.
        {
            forwardMovement += mMoveDirection;
            if (mGameManager.GetSquare(forwardMovement)->IsEmpty())
                AddMoveLocation(forwardMovement); // 공격은 할 수 없지만 이동은 할 수 있기 때문에 Move 배열에 추가
        }
    }
}

void Pawn::SearchDiagonalDirection(const Coordinates2& direction)
{
    // TODO: 이동했을 때 체크가 발생하는지 확인
    const Coordinates2 diagonalMovement = mCurrentPosition + direction;
    // 대각선 방향이 체스판 범위를 벗어났다면 탐색 종료
    if (!GameManager::ValidIndex(diagonalMovement)) return;
    //대각선에 적군이 있다면
    if (mGameManager.GetSquare(diagonalMovement)->GetColor() == mEnemyColor)
    {
        AddAttackLocation(diagonalMovement); // 공격 가능 지역 추가
        AddMoveLocation(diagonalMovement); // 이동 가능 지역에 추가
    }
    else // 비었거나 아군이 있다면
    {
        AddAttackLocation(diagonalMovement); // 공격 가능 지역 추가
    }
}

void Pawn::SearchEnpassantDirection(const Coordinates2& direction)
{
    // TODO: 이동했을 때 체크가 발생하는지 확인

    const Coordinates2 sidePosition(direction.x + mCurrentPosition.x, mCurrentPosition.y);
    const Coordinates2 movePosition(direction + mCurrentPosition);
    // 보드 범위를 벗어날 경우 종료
    if (!GameManager::ValidIndex(sidePosition)) return;

    auto sidePiece = mGameManager.GetSquare(sidePosition)->GetPiece().lock();
    // direction방향의 옆쪽 piece가 존재하고, piece 타입이 pawn 이고 적군이라면
    if (sidePiece && sidePiece->GetType() == PieceType::Pawn && sidePiece->GetColor() == mEnemyColor)
    {
        auto enemyPawn = std::static_pointer_cast<Pawn>(sidePiece);
        if (enemyPawn->IsMoveTwoSquare()) // 앙파상 가능
        {
            AddAttackLocation(sidePosition);
            AddMoveLocation(movePosition);
        }
    }
}

const std::array<Coordinates2, 8> Knight::KnightMovements 
    = { Coordinates2(1, 2), Coordinates2(2, 1), Coordinates2(-1, 2), Coordinates2(2, -1), 
    Coordinates2(1, -2), Coordinates2(-2, 1), Coordinates2(-1, -2), Coordinates2(-2, -1) };

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
void Knight::SearchAttackAndMoveLocation()
{
    for (int i = 0; i < 8; ++i)
    {
        Coordinates2 next = mCurrentPosition + KnightMovements[i];
        // TODO: 이동했을 때 체크가 발생하는지 확인
        if (GameManager::ValidIndex(next)) // 유효한 좌표이며, 
        {
            PieceColor piceColor = mGameManager.GetSquare(next)->GetColor();
            // 해당 칸에 아군이 있다면 공격은 가능하지만 이동은 불가능
            if (piceColor == mColor)
            {
                AddMoveLocation(next);
            }
            else // 해당 칸에 적군이 있거나 비어있다면, 공격 이동 모두 가능
            {
                AddAttackLocation(next);
                AddMoveLocation(next);
            }
        }

    }
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

void Bishop::SearchAttackAndMoveLocation()
{
    // 대각선 4방향 모두 탐색
    for (int i = 0; i < 4; ++i)
    {
        SearchInTheDirection(Piece::DiagonalDirections[i]);
    }
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

void Rook::SearchAttackAndMoveLocation()
{
    // 직선 4방향 모두 탐색
    for (int i = 0; i < 4; ++i)
    {
        SearchInTheDirection(Piece::StraightDirections[i]);
    }
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

void Queen::SearchAttackAndMoveLocation()
{
    // 직선 4방향, 대각선 4방향 모두 탐색
    for (int i = 0; i < 4; ++i)
    {
        SearchInTheDirection(Piece::StraightDirections[i]);
        SearchInTheDirection(Piece::DiagonalDirections[i]);
    }
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
void King::SearchAttackAndMoveLocation()
{
    // 인접위치
    for (int i = 0; i < 4; ++i)
    {
        SearchAdjacent(DiagonalDirections[i]); // 대각선 4방향
        SearchAdjacent(StraightDirections[i]); // 직선 4방향
    }
    // 캐슬링 가능 여부 탐색
    SearchCastlingKingSide();
    SearchCastlingQueenSide();
}
void King::MovePieceTo(const Coordinates2 & nextPosition)
{
    // 두칸 움직였다면 캐슬링
    if (nextPosition.x - mCurrentPosition.x == 2)
        DoCastlingKingSide();
    else if (nextPosition.x - mCurrentPosition.x == -2)
        DoCastlingQueenSide();

    // 이동
    Piece::MovePieceTo(nextPosition);
}
void King::SearchAdjacent(const Coordinates2& direction)
{
    auto next = mCurrentPosition + direction;
    if (GameManager::ValidIndex(next)) // 유효한 위치일때
    {
        PieceColor nextColor = mGameManager.GetSquare(next)->GetColor();
        if (nextColor == mColor)  // 아군: 공격만 가능
        {
            AddAttackLocation(next);
        }
        else // 빈칸or 적군: 공격, 이동 가능
        {
            // TODO: 이동했을 때 체크가 발생하는지 확인
            AddAttackLocation(next);
            AddMoveLocation(next);
        }
    }
}

void King::SearchCastlingKingSide()
{
    auto KingSquare = mGameManager.GetSquare(mCurrentPosition);
    if (mbFirstMove == true && KingSquare->IsAttacked(mEnemyColor) == false) // 킹이 한번도 움직인적 없으며, 공격받고 있지 아니하다면
    {
        auto Rook = mGameManager.GetSquare(Coordinates2 (7, mCurrentPosition.y))->GetPiece().lock();
        auto BishopSquare = mGameManager.GetSquare(Coordinates2 (5, mCurrentPosition.y));
        auto KnightSquare = mGameManager.GetSquare(Coordinates2 (6, mCurrentPosition.y));
        if (Rook && Rook->GetFirstMove()  // king side에 Rook이 존재하고, 움직인적이 없다면,
            && BishopSquare->IsEmpty() && !BishopSquare->IsAttacked(mEnemyColor) // 비숍칸이 비어있고, 공격받고 있지 아니하며
            && KnightSquare->IsEmpty() && !KnightSquare->IsAttacked(mEnemyColor)) // 나이트 칸도 비어있고, 공격받고 있지 아니하다면
        {
            AddMoveLocation(Coordinates2(6, mCurrentPosition.y)); // 킹 사이드 나이트 위치로 이동 가능
        }
    }
}

void King::SearchCastlingQueenSide()
{
    auto KingSquare = mGameManager.GetSquare(mCurrentPosition);
    if (mbFirstMove == true && KingSquare->IsAttacked(mEnemyColor) == false) // 킹이 한번도 움직인적 없으며, 공격받고 있지 아니하다면
    {
        auto Rook = mGameManager.GetSquare(Coordinates2(0, mCurrentPosition.y))->GetPiece().lock();
        auto QueenSquare = mGameManager.GetSquare(Coordinates2(3, mCurrentPosition.y));
        auto BishopSquare = mGameManager.GetSquare(Coordinates2(2, mCurrentPosition.y));
        if (Rook && Rook->GetFirstMove()  // Queen side에 Rook이 존재하고, 움직인적이 없다면,
            && QueenSquare->IsEmpty() && !QueenSquare->IsAttacked(mEnemyColor) // 퀸 칸이 비어있고, 공격받고 있지 아니하고
            && BishopSquare->IsEmpty() && !BishopSquare->IsAttacked(mEnemyColor)) // 비숍칸이 비어있고, 공격받고 있지 아니하다면
        {
            AddMoveLocation(Coordinates2(2, mCurrentPosition.y)); // 퀸사이드 비숍 위치로 이동 가능
        }
    }
}

void King::DoCastlingKingSide()
{
    auto Rook = mGameManager.GetSquare(Coordinates2(7, mCurrentPosition.y))->GetPiece().lock();
    Rook->MovePieceTo(Coordinates2(5, mCurrentPosition.y));
}

void King::DoCastlingQueenSide()
{
    auto Rook = mGameManager.GetSquare(Coordinates2(0, mCurrentPosition.y))->GetPiece().lock();
    Rook->MovePieceTo(Coordinates2(3, mCurrentPosition.y));
}
