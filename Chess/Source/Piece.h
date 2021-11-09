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
    void MovePieceTo(const Coordinates2& nextCoordinates, bool bIsUpdateAllNextPosition = true);
    // ���� �̵���ġ�� �����ϴ� �������̽� �Լ�. �ϳ��� �⹰�� �̵��� ��� ��� �⹰�� NextPosition�� �����ϰԵ�
    virtual void UpdateNextPosition() = 0;
    // Actor�� ������ ���� ȣ��Ǵ� ��ӹ��� �ʱ�ȭ �Լ�. �⹰�� ũ��� ��ġ�� ����
    void Initialize() override;
    // �⹰�� ���ݹ������ ȣ��
    void BeAttacked();
    // �⹰�� ����Ȯ��
    bool IsBlack() const { return mColor == PieceColor::Black; }

    // Getter & Setter
    std::weak_ptr<SpriteComponent> GetSpriteComponent() const { return mSpriteComponent; }
    PieceType GetType() const { return mType; }
    PieceColor GetColor() const { return mColor; }
    Coordinates2 GetPosition() const { return mCurrentPosition; }

private:
    // GameManager
    GameManager& mGameManager;    
    // �⹰�� sprite
    std::weak_ptr<SpriteComponent> mSpriteComponent;
    // �⹰�� ���� ��ǥ (���� ��ǥ��)
    Coordinates2 mCurrentPosition;
    // �⹰�� ���� �̵� ������ ��ġ
    std::vector<Coordinates2> mNextPosition;
    // �⹰�� Ÿ��
    PieceType mType;
    // �⹰�� ����
    PieceColor mColor;
    // �⹰�� ũ�� (�ȼ� ����)
    size_t mSize;
};

class Pawn : public Piece
{
public:
    Pawn(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Pawn();

    void UpdateNextPosition() override {}

private:
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







