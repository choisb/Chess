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
    for (const auto& next : mNextPosition)
    {
        mGameManager.GetSquare(next)->CancelCandidate();
    }

    // �ش� ��ġ�� �̵�
    mCurrentPosition = nextPosition;
    SetActorLocation(mGameManager.GetActorLocationOf(mCurrentPosition));

    // �ش� ��ġ ĭ�� ǥ��
    square = mGameManager.GetSquare(mCurrentPosition);
    square->Occupied(std::static_pointer_cast<Piece>(shared_from_this()));
}
void Piece::StartTurn()
{
    //�⹰�� next position�� ����
    UpdateNextPosition();
}
void Piece::BeAttacked()
{
    SetState(State::EDead);
    mGameManager.RemovePiece(std::static_pointer_cast<Piece>(shared_from_this()), mColor);
}

void Piece::Selected()
{
    // ���� ��ġ Square ����
    mGameManager.GetSquare(mCurrentPosition)->Selected();
    // �ĺ� ��ġ Square ��� �ĺ����� ����
    for (auto& next : mNextPosition)
    {
        mGameManager.GetSquare(next)->BeCandidate();
    }
}

void Piece::Unselected()
{
    // ���� ��ġ Square ���� ����
    mGameManager.GetSquare(mCurrentPosition)->Unselected();
    // �ĺ� ��ġ Square ��� �ĺ��� ����
    for (auto& next : mNextPosition)
    {
        mGameManager.GetSquare(next)->CancelCandidate();
    }
}


// Pawn ����
Pawn::Pawn(Game& game, GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size /*=128*/)
    : Piece(game, gameManager, PieceType::Pawn, inColor, inPosition, size)
    , mMoveDirection(0,1) // ���� ��� (0,1)
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
    Coordinates2 side(nextPosition.x, mCurrentPosition.y);

    // ���Ļ� ����, ���Ļ��� �´ٸ� ���Ļ� ����
    if (IsThisMoveEnpassant(side))
    {
        DoEnpassant(side);
    }

    // �̵�
    Piece::MovePieceTo(nextPosition);

    // ù��° ������ ����
    mbFirstMove = true;
    //TODO: �� ���θ�� ��� �߰�, UI ��� ���� �� ���� 
}
bool Pawn::IsThisMoveEnpassant(const Coordinates2& side)
{
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
    UpdateDiagonalDirection(Coordinates2(1, 0)); // ���� �밢�� ���� Ž��
    UpdateDiagonalDirection(Coordinates2(-1, 0)); // ���� �밢�� ���� Ž��
    UpdateEnpassantDirection(Coordinates2(1,0)); // �������� ���Ļ�
    UpdateEnpassantDirection(Coordinates2(-1,0)); // �������� ���Ļ�
}

void Pawn::UpdateForwardDirection()
{
    // ������ ����ٸ�
    Coordinates2 forward = mCurrentPosition + mMoveDirection;

    if (GameManager::ValidIndex(forward) && mGameManager.GetSquare(forward)->IsEmpty())
    {
        mNextPosition.push_back(forward);
        if (mbFirstMove == false) // �ѹ��� �������� ���ٸ� ��ĭ ������ �� �ִ�.
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
    // �밢�� ������ ������ ����ٸ� Ž�� ����
    if (!GameManager::ValidIndex(diagonal)) return;
    //�밢���� ������ �ִٸ�
    if (mGameManager.GetSquare(diagonal)->GetColor() == mEnemyColor)
    {
        mNextPosition.push_back(diagonal);
    }
}

void Pawn::UpdateEnpassantDirection(const Coordinates2& sideDirection)
{
    const Coordinates2 side = mCurrentPosition + sideDirection;
    // ���� ������ ��� ��� ����
    if (!GameManager::ValidIndex(side)) return;

    auto sidePiece = mGameManager.GetSquare(side)->GetPiece().lock();
    // direction������ ���� piece�� �����ϰ�, piece Ÿ���� pawn �̶��
    if (sidePiece && sidePiece->GetType() == PieceType::Pawn)
    {
        auto pawn = std::static_pointer_cast<Pawn>(sidePiece);
        if (pawn->IsMoveTwoSquare()) // ���Ļ� ����
            mNextPosition.push_back(side + mMoveDirection);
    }
}

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