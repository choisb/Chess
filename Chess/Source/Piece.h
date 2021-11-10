#pragma once
#include "Core/Actor.h"
#include "Core/Math.h"
#include "PieceTypes.h"
#include <memory>
#include <vector>

class SpriteComponent;

class Piece : public Actor
{
public:
    Piece(class Game& game, class GameManager& gameManager, PieceType type, PieceColor color, Coordinates2 position, size_t size);
    ~Piece();

    // �⹰�� �Է¹��� ���� ��ǥ�� �̵��ϴ� �Լ� (bIsUpdateAllNextPosition == false �� ��� �̵� �� ���� ����)
    virtual void MovePieceTo(const Coordinates2& nextPosition);
    // ���� �����Ҷ� ȣ��Ǵ� �Լ�
    virtual void StartTurn();
    // Actor�� ������ ���� ȣ��Ǵ� ��ӹ��� �ʱ�ȭ �Լ�. �⹰�� ũ��� ��ġ�� ����
    void Initialize() override;
    // �⹰�� ���ݹ������ ȣ��
    void BeAttacked();
    // �⹰�� ����Ȯ��
    bool IsBlack() const { return mColor == PieceColor::Black; }
    // �⹰�� ���õ� ��� ȣ���
    void Selected();
    // �⹰�� ���� ������ ��� ȣ���
    void Unselected();
    
    // Getter & Setter
    std::weak_ptr<SpriteComponent> GetSpriteComponent() const { return mSpriteComponent; }
    PieceType GetType() const { return mType; }
    PieceColor GetColor() const { return mColor; }
    Coordinates2 GetPosition() const { return mCurrentPosition; }
    void SetPosition(const Coordinates2& inPosition) { mCurrentPosition = inPosition; }

protected:
    // GameManager
    GameManager& mGameManager;    
    // �⹰�� ���� ��ǥ (���� ��ǥ��)
    Coordinates2 mCurrentPosition;
    // �⹰�� Ÿ��
    PieceType mType;
    // �⹰�� ����
    PieceColor mColor;
    // ������ ����
    PieceColor mEnemyColor;
    // �⹰�� ũ�� (�ȼ� ����)
    size_t mSize;

    // �⹰�� ���� �̵� ������ ��ġ
    std::vector<Coordinates2> mNextPosition;
    // �⹰�� sprite
    std::weak_ptr<SpriteComponent> mSpriteComponent;

    // ���� �̵���ġ�� �����ϴ� �������̽� �Լ�. ���� ���۵ɶ� StartTurn() �Լ��� ���ؼ� ȣ��ȴ�.
    virtual void UpdateNextPosition() = 0;
};

class Pawn : public Piece
{
public:
    Pawn(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Pawn();

    // �ش� ��ġ�� �̵��ϴ� �Լ�. Piece::MovePieceTo �� �ݵ�� ȣ���ؾ���.
    void MovePieceTo(const Coordinates2& nextPosition) override;
    // Turn�� �����Ҷ� ȣ��Ǵ� �Լ�. Piece::StartTurn�� �ݵ�� ȣ���ؾ���.
    void StartTurn() override;

    //Setter & Getter 
    bool IsMoveTwoSquare() { return mbMoveTwoSquare; }
    
private:
    // ���� ��ġ�� �����ϴ� �Լ�
    void UpdateNextPosition() override;
    // �̹� �������� ���Ļ��̶�� true
    bool IsThisMoveEnpassant(const Coordinates2& side);
    // ���Ļ� ����
    void DoEnpassant(const Coordinates2& side);
    // ���� ���� ��ġ�� �����ϴ� helper �Լ���
    void UpdateForwardDirection();
    void UpdateDiagonalDirection(const Coordinates2& sideDirection);
    void UpdateEnpassantDirection(const Coordinates2& sideDirection);

    // ���� �̵� ����(black�ϰ�� + / whtie �ϰ�� -)
    Coordinates2 mMoveDirection;
    // �� �� �����̸� true
    bool mbFirstMove;
    // ���Ļ� ������ ���� ���. ��ĭ �������ٸ� true, ���� �� ���۵ɶ� false
    bool mbMoveTwoSquare;
    // �̹��Ͽ� ���Ļ��� ������ �� �ִٸ� true
    bool mbCanEnpassant;
};

class Knight : public Piece
{
public:
    Knight(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Knight();

    void UpdateNextPosition() override {}
};

class Bishop : public Piece
{
public:
    Bishop(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Bishop();

    void UpdateNextPosition() override {}
};

class Rook : public Piece
{
public:
    Rook(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Rook();

    void UpdateNextPosition() override {}
};


class Queen : public Piece
{
public:
    Queen(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Queen();

    void UpdateNextPosition() override {}
};


class King : public Piece
{
public:
    King(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~King();

    void UpdateNextPosition() override {}
};







