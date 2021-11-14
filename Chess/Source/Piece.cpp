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
    // ��������Ʈ ũ�� ����
    auto sc = GetSpriteComponent().lock();
    if (sc)
    {
        const float textureSize = static_cast<float>(sc->GetTexHeight());
        const float newScale = static_cast<float>(mSize) / textureSize;
        SetActorScale(newScale);
    }

    // �⹰ ��ġ �ʱ�ȭ 
    SetActorLocation(mGameManager.GetActorLocationOf(mCurrentPosition));

    // �ش� ĭ�� �⹰ ���� ǥ��
    auto square = mGameManager.GetSquare(mCurrentPosition);
    square->Occupied(std::static_pointer_cast<Piece>(shared_from_this()));
}
void Piece::MovePieceTo(const Coordinates2& nextPosition)
{
    // ���� ĭ UnOccupied
    auto square = mGameManager.GetSquare(mCurrentPosition);
    square->UnOccupied();
    square->Unselected();

    // �̵��ϱ� �� �̵� �ĺ����� ��ŷ�Ǿ��ִ� Square ��� �ĺ����� ����
    for (const auto& candidate : mMoveLocations)
    {
        mGameManager.GetSquare(candidate)->CancelCandidate();
    }

    // �ش� ��ġ�� �̵�
    mCurrentPosition = nextPosition;
    SetActorLocation(mGameManager.GetActorLocationOf(mCurrentPosition));

    // �̵��Ϸ��� square
    square = mGameManager.GetSquare(mCurrentPosition);
    // �̵��Ϸ��� square�� �⹰�� �ִ°�� �⹰ �ı�
    if (auto currentPiece = square->GetPiece().lock())
    {
        currentPiece->BeDestroyed();
    }
    // ���� ǥ��
    square->Occupied(std::static_pointer_cast<Piece>(shared_from_this()));

    // ù��° ������ ����
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
    // ���� ��ġ Square ����
    mGameManager.GetSquare(mCurrentPosition)->Selected();
    // �ĺ� ��ġ Square ��� �ĺ����� ����
    for (auto& next : mMoveLocations)
    {
        mGameManager.GetSquare(next)->BeCandidate();
    }
}

void Piece::Unselected()
{
    // ���� ��ġ Square ���� ����
    mGameManager.GetSquare(mCurrentPosition)->Unselected();
    // �ĺ� ��ġ Square ��� �ĺ��� ����
    for (auto& next : mMoveLocations)
    {
        mGameManager.GetSquare(next)->CancelCandidate();
    }
}

void Piece::AddAttackLocation(const Coordinates2 & position)
{
    auto square = mGameManager.GetSquare(position);
    // �����ϴ� ������ ������ ŷ�� �ִ� ���̶�� Check �߻�
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
    // TODO: �̵� �����ϴٸ� ��, �̵����� �� üũ�� �߻����� �ʴ´ٸ� �̵�
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
        if (nextColor == PieceColor::Default_MAX)  // ��ĭ: ����, �̵� ���� Ž�� ���
        {
            // TODO: �̵����� �� üũ�� �߻��ϴ��� Ȯ��
            AddAttackLocation(next);
            AddMoveLocation(next);
            next += direction;
        }
        else if (nextColor == mEnemyColor) // ����: ����, �̵� ����, Ž�� ����
        {
            // TODO: �̵����� �� üũ�� �߻��ϴ��� Ȯ��
            AddAttackLocation(next);
            AddMoveLocation(next);
            break;
        }
        else // �Ʊ�: ���ݸ� ����
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


// Pawn ����
Pawn::Pawn(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Pawn, inColor, inPosition, size)
    , mMoveDirection(0,1) // ���� ��� (0,1)
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
 
    // white�� ��� (0, -1)�� ����
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

    // ���Ļ� ����, ���Ļ��� �´ٸ� ���Ļ� ����
    if (IsThisMovementEnpassant(nextPosition))
    {
        DoEnpassant(nextPosition);
    }

    // �̵�
    Piece::MovePieceTo(nextPosition);

    //TODO: �� ���θ�� ��� �߰�, UI ��� ���� �� ���� 
}
bool Pawn::IsThisMovementEnpassant(const Coordinates2& nextPosition)
{
    Coordinates2 side(nextPosition.x, mCurrentPosition.y);
    auto sidePiece = mGameManager.GetSquare(side)->GetPiece().lock();
    // �밢������ �̵��ϰ�, �̵��ϴ� ���� ���� �⹰�� �����ϸ�, �� �⹰�� ���� �̸�, ���� �⹰�� ���ΰ��
    if (side.x != mCurrentPosition.x && sidePiece && sidePiece->GetColor() != mColor && sidePiece->GetType() == PieceType::Pawn)
    {
        auto enemyPawn = std::static_pointer_cast<Pawn>(sidePiece);
        // �� Pawn�� ���� �Ͽ� �� ĭ�̻� �������ٸ� ���Ļ��̱� ������ ����
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
    SearchDiagonalDirection(Coordinates2(1, mMoveDirection.y)); // ���� �밢�� ���� Ž��
    SearchDiagonalDirection(Coordinates2(-1, mMoveDirection.y)); // ���� �밢�� ���� Ž��
    SearchEnpassantDirection(Coordinates2(1, mMoveDirection.y)); // �������� ���Ļ�
    SearchEnpassantDirection(Coordinates2(-1, mMoveDirection.y)); // �������� ���Ļ�
}



void Pawn::SearchForwardDirection()
{
    // TODO: �̵����� �� üũ�� �߻��ϴ��� Ȯ��
    // ������ ����ٸ�
    Coordinates2 forwardMovement = mCurrentPosition + mMoveDirection;

    if (GameManager::ValidIndex(forwardMovement) && mGameManager.GetSquare(forwardMovement)->IsEmpty())
    {
        AddMoveLocation(forwardMovement); // ������ �� �� ������ �̵��� �� �� �ֱ� ������ Move �迭�� �߰�
        if (mbFirstMove == true) // �ѹ��� ���������� ���ٸ� ��ĭ ������ �� �ִ�.
        {
            forwardMovement += mMoveDirection;
            if (mGameManager.GetSquare(forwardMovement)->IsEmpty())
                AddMoveLocation(forwardMovement); // ������ �� �� ������ �̵��� �� �� �ֱ� ������ Move �迭�� �߰�
        }
    }
}

void Pawn::SearchDiagonalDirection(const Coordinates2& direction)
{
    // TODO: �̵����� �� üũ�� �߻��ϴ��� Ȯ��
    const Coordinates2 diagonalMovement = mCurrentPosition + direction;
    // �밢�� ������ ü���� ������ ����ٸ� Ž�� ����
    if (!GameManager::ValidIndex(diagonalMovement)) return;
    //�밢���� ������ �ִٸ�
    if (mGameManager.GetSquare(diagonalMovement)->GetColor() == mEnemyColor)
    {
        AddAttackLocation(diagonalMovement); // ���� ���� ���� �߰�
        AddMoveLocation(diagonalMovement); // �̵� ���� ������ �߰�
    }
    else // ����ų� �Ʊ��� �ִٸ�
    {
        AddAttackLocation(diagonalMovement); // ���� ���� ���� �߰�
    }
}

void Pawn::SearchEnpassantDirection(const Coordinates2& direction)
{
    // TODO: �̵����� �� üũ�� �߻��ϴ��� Ȯ��

    const Coordinates2 sidePosition(direction.x + mCurrentPosition.x, mCurrentPosition.y);
    const Coordinates2 movePosition(direction + mCurrentPosition);
    // ���� ������ ��� ��� ����
    if (!GameManager::ValidIndex(sidePosition)) return;

    auto sidePiece = mGameManager.GetSquare(sidePosition)->GetPiece().lock();
    // direction������ ���� piece�� �����ϰ�, piece Ÿ���� pawn �̰� �����̶��
    if (sidePiece && sidePiece->GetType() == PieceType::Pawn && sidePiece->GetColor() == mEnemyColor)
    {
        auto enemyPawn = std::static_pointer_cast<Pawn>(sidePiece);
        if (enemyPawn->IsMoveTwoSquare()) // ���Ļ� ����
        {
            AddAttackLocation(sidePosition);
            AddMoveLocation(movePosition);
        }
    }
}

const std::array<Coordinates2, 8> Knight::KnightMovements 
    = { Coordinates2(1, 2), Coordinates2(2, 1), Coordinates2(-1, 2), Coordinates2(2, -1), 
    Coordinates2(1, -2), Coordinates2(-2, 1), Coordinates2(-1, -2), Coordinates2(-2, -1) };

// Knight ����
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
        // TODO: �̵����� �� üũ�� �߻��ϴ��� Ȯ��
        if (GameManager::ValidIndex(next)) // ��ȿ�� ��ǥ�̸�, 
        {
            PieceColor piceColor = mGameManager.GetSquare(next)->GetColor();
            // �ش� ĭ�� �Ʊ��� �ִٸ� ������ ���������� �̵��� �Ұ���
            if (piceColor == mColor)
            {
                AddMoveLocation(next);
            }
            else // �ش� ĭ�� ������ �ְų� ����ִٸ�, ���� �̵� ��� ����
            {
                AddAttackLocation(next);
                AddMoveLocation(next);
            }
        }

    }
}

// Bishop ����
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
    // �밢�� 4���� ��� Ž��
    for (int i = 0; i < 4; ++i)
    {
        SearchInTheDirection(Piece::DiagonalDirections[i]);
    }
}


// Rook ����
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
    // ���� 4���� ��� Ž��
    for (int i = 0; i < 4; ++i)
    {
        SearchInTheDirection(Piece::StraightDirections[i]);
    }
}



// Queen ����
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
    // ���� 4����, �밢�� 4���� ��� Ž��
    for (int i = 0; i < 4; ++i)
    {
        SearchInTheDirection(Piece::StraightDirections[i]);
        SearchInTheDirection(Piece::DiagonalDirections[i]);
    }
}

// King ����
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
    // ������ġ
    for (int i = 0; i < 4; ++i)
    {
        SearchAdjacent(DiagonalDirections[i]); // �밢�� 4����
        SearchAdjacent(StraightDirections[i]); // ���� 4����
    }
    // ĳ���� ���� ���� Ž��
    SearchCastlingKingSide();
    SearchCastlingQueenSide();
}
void King::MovePieceTo(const Coordinates2 & nextPosition)
{
    // ��ĭ �������ٸ� ĳ����
    if (nextPosition.x - mCurrentPosition.x == 2)
        DoCastlingKingSide();
    else if (nextPosition.x - mCurrentPosition.x == -2)
        DoCastlingQueenSide();

    // �̵�
    Piece::MovePieceTo(nextPosition);
}
void King::SearchAdjacent(const Coordinates2& direction)
{
    auto next = mCurrentPosition + direction;
    if (GameManager::ValidIndex(next)) // ��ȿ�� ��ġ�϶�
    {
        PieceColor nextColor = mGameManager.GetSquare(next)->GetColor();
        if (nextColor == mColor)  // �Ʊ�: ���ݸ� ����
        {
            AddAttackLocation(next);
        }
        else // ��ĭor ����: ����, �̵� ����
        {
            // TODO: �̵����� �� üũ�� �߻��ϴ��� Ȯ��
            AddAttackLocation(next);
            AddMoveLocation(next);
        }
    }
}

void King::SearchCastlingKingSide()
{
    auto KingSquare = mGameManager.GetSquare(mCurrentPosition);
    if (mbFirstMove == true && KingSquare->IsAttacked(mEnemyColor) == false) // ŷ�� �ѹ��� �������� ������, ���ݹް� ���� �ƴ��ϴٸ�
    {
        auto Rook = mGameManager.GetSquare(Coordinates2 (7, mCurrentPosition.y))->GetPiece().lock();
        auto BishopSquare = mGameManager.GetSquare(Coordinates2 (5, mCurrentPosition.y));
        auto KnightSquare = mGameManager.GetSquare(Coordinates2 (6, mCurrentPosition.y));
        if (Rook && Rook->GetFirstMove()  // king side�� Rook�� �����ϰ�, ���������� ���ٸ�,
            && BishopSquare->IsEmpty() && !BishopSquare->IsAttacked(mEnemyColor) // ���ĭ�� ����ְ�, ���ݹް� ���� �ƴ��ϸ�
            && KnightSquare->IsEmpty() && !KnightSquare->IsAttacked(mEnemyColor)) // ����Ʈ ĭ�� ����ְ�, ���ݹް� ���� �ƴ��ϴٸ�
        {
            AddMoveLocation(Coordinates2(6, mCurrentPosition.y)); // ŷ ���̵� ����Ʈ ��ġ�� �̵� ����
        }
    }
}

void King::SearchCastlingQueenSide()
{
    auto KingSquare = mGameManager.GetSquare(mCurrentPosition);
    if (mbFirstMove == true && KingSquare->IsAttacked(mEnemyColor) == false) // ŷ�� �ѹ��� �������� ������, ���ݹް� ���� �ƴ��ϴٸ�
    {
        auto Rook = mGameManager.GetSquare(Coordinates2(0, mCurrentPosition.y))->GetPiece().lock();
        auto QueenSquare = mGameManager.GetSquare(Coordinates2(3, mCurrentPosition.y));
        auto BishopSquare = mGameManager.GetSquare(Coordinates2(2, mCurrentPosition.y));
        if (Rook && Rook->GetFirstMove()  // Queen side�� Rook�� �����ϰ�, ���������� ���ٸ�,
            && QueenSquare->IsEmpty() && !QueenSquare->IsAttacked(mEnemyColor) // �� ĭ�� ����ְ�, ���ݹް� ���� �ƴ��ϰ�
            && BishopSquare->IsEmpty() && !BishopSquare->IsAttacked(mEnemyColor)) // ���ĭ�� ����ְ�, ���ݹް� ���� �ƴ��ϴٸ�
        {
            AddMoveLocation(Coordinates2(2, mCurrentPosition.y)); // �����̵� ��� ��ġ�� �̵� ����
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
