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

    // 기물을 입력받은 정수 좌표로 이동하는 함수 (bIsUpdateAllNextPosition == false 일 경우 이동 후 갱신 안함)
    void MovePieceTo(const Coordinates2& nextCoordinates, bool bIsUpdateAllNextPosition = true);
    // 다음 이동위치를 갱신하는 인터페이스 함수. 하나의 기물이 이동할 경우 모든 기물의 NextPosition을 갱신하게됨
    virtual void UpdateNextPosition() = 0;
    // Actor가 생성된 직후 호출되는 상속받은 초기화 함수. 기물의 크기와 위치를 설정
    void Initialize() override;
    // 기물이 공격받을경우 호출
    void BeAttacked();
    // 기물의 색상확인
    bool IsBlack() const { return mColor == PieceColor::Black; }

    // Getter & Setter
    std::weak_ptr<SpriteComponent> GetSpriteComponent() const { return mSpriteComponent; }
    PieceType GetType() const { return mType; }
    PieceColor GetColor() const { return mColor; }
    Coordinates2 GetPosition() const { return mCurrentPosition; }

private:
    // GameManager
    GameManager& mGameManager;    
    // 기물의 sprite
    std::weak_ptr<SpriteComponent> mSpriteComponent;
    // 기물의 현재 좌표 (정수 좌표계)
    Coordinates2 mCurrentPosition;
    // 기물이 다음 이동 가능한 위치
    std::vector<Coordinates2> mNextPosition;
    // 기물의 타입
    PieceType mType;
    // 기물의 색상
    PieceColor mColor;
    // 기물의 크기 (픽셀 단위)
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







