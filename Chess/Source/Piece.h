#pragma once
#include "Core/Actor.h"
#include "Core/Math.h"
#include "PieceTypes.h"
#include <memory>
#include <vector>
#include <array>

class SpriteComponent;

class Piece : public Actor
{
public:
    Piece(class Game& game, class GameManager& gameManager, PieceType type, PieceColor color, Coordinates2 position, size_t size);
    ~Piece();

    // Actor�� ������ ���� ȣ��Ǵ� ��ӹ��� �ʱ�ȭ �Լ�. �⹰�� ũ��� ��ġ�� ����
    void Initialize() override;
    // �⹰�� �Է¹��� ���� ��ǥ�� �̵��ϴ� �Լ� (bIsUpdateAllNextPosition == false �� ��� �̵� �� ���� ����)
    virtual void MovePieceTo(const Coordinates2& nextPosition);
    // ���� ���۵� �� ȣ��Ǵ� �����Լ�
    virtual void StartTurn() {}
    // �⹰�� ���ݹ������ ȣ��
    void IsDestroyed();

    // �⹰�� ���õ� ��� ȣ��� (���� ���� ����ȭ)
    void Selected();
    // �⹰�� ���� ������ ��� ȣ��� (���� ���� ����ȭ)
    void Unselected();

    // �ش� ������ �����Ѵ�.
    void AddAttackLocation(const Coordinates2& position);
    // ���� mOnlyMoveLocation �ʱ�ȭ�� ���� ����ȭ
    void ReleaseFromAllAttacks();

    // ���� �̵� �迭�� �߰��ϴ� �Լ�
    void AddMoveLocation(const Coordinates2& position);
    // mMoveLocation �迭 �ʱ�ȭ
    void ReleaseMoveLocation();


    // ���� al���� ������ ��� Ž��
    virtual void SearchAttackAndMoveLocation() = 0;

    // Getter & Setter
    std::weak_ptr<SpriteComponent> GetSpriteComponent() const { return mSpriteComponent; }
    PieceType GetType() const { return mType; }
    PieceColor GetColor() const { return mColor; }
    Coordinates2 GetPosition() const { return mCurrentPosition; }
    void SetPosition(const Coordinates2& inPosition) { mCurrentPosition = inPosition; }
    bool GetFirstMove() const { return mbFirstMove; }

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
    // ù �����Ӹ� true, �ѹ� �������� �ִٸ� false
    bool mbFirstMove;
    // �⹰�� sprite
    std::weak_ptr<SpriteComponent> mSpriteComponent;

    // ���� �̵���ġ�� Ž���ϴ� �������̽� �Լ�. ���� ���۵ɶ� StartTurn() �Լ��� ���ؼ� ȣ��ȴ�.
    void SearchInTheDirection(const Coordinates2& direction);
    // �ش� ��ġ�� �̵� ��������, üũ�� �߻����� �ʴ��� Ȯ���Ͽ� true / false ��ȯ
    bool CanMoveTo(const Coordinates2& position);

    static const std::array<Coordinates2, 4> StraightDirections;
    static const std::array<Coordinates2, 4> DiagonalDirections;

private:
    // �⹰�� �������� ��ġ �������̽��θ� �����ؾ���.
    std::vector<Coordinates2> mLocationBeingAttacked;
    // ������ ������ �� �ִ� ��ġ �������̽��θ� �����ؾ���.
    std::vector<Coordinates2> mMoveLocation;
};

class Pawn : public Piece
{
public:
    Pawn(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Pawn();

    // �ش� ��ġ�� �̵��ϴ� �Լ�. Piece::MovePieceTo �� �ݵ�� ȣ���ؾ���.
    void MovePieceTo(const Coordinates2& nextPosition) override;
    // ���� �����ϰ� ȣ��Ǵ� �����Լ�
    void StartTurn() override;
    //Setter & Getter 
    bool IsMoveTwoSquare() { return mbMoveTwoSquare; }
    // ���� ��ġ�� Ž���ϴ� �Լ�
    void SearchAttackAndMoveLocation() override;

private:

    // �̹� �������� ���Ļ��̶�� true
    bool IsThisMovementEnpassant(const Coordinates2& side);
    // ���Ļ� ����
    void DoEnpassant(const Coordinates2& side);
    // ���� ���� ��ġ�� Ž���ϴ� helper �Լ���
    void SearchForwardDirection();
    void SearchDiagonalDirection(const Coordinates2& direction);
    void SearchEnpassantDirection(const Coordinates2& direction);

    // ���� �̵� ����(black�ϰ�� + / whtie �ϰ�� -)
    Coordinates2 mMoveDirection;
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

    void SearchAttackAndMoveLocation() override;

private:
    static const std::array<Coordinates2, 8> KnightMovements;
};

class Bishop : public Piece
{
public:
    Bishop(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Bishop();

    void SearchAttackAndMoveLocation() override;
};

class Rook : public Piece
{
public:
    Rook(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Rook();

    void SearchAttackAndMoveLocation() override;
private:
    // TODO: ĳ���� ����
};


class Queen : public Piece
{
public:
    Queen(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Queen();

    void SearchAttackAndMoveLocation() override;
};


class King : public Piece
{
public:
    King(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~King();

    void SearchAttackAndMoveLocation() override;
    void MovePieceTo(const Coordinates2& nextPosition) override;
private:
    // ���� ��ġ Ž��
    void SearchAdjacent(const Coordinates2& direction);
    // ĳ���� Ž��
    void SearchCastlingKingSide();
    void SearchCastlingQueenSide();
    // ĳ���� ����
    void DoCastlingKingSide();
    void DoCastlingQueenSide();
};







