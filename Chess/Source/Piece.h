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
    virtual void MovePieceTo(const Coordinates2& nextPosition);
    // 턴을 시작할때 호출되는 함수
    virtual void StartTurn();
    // Actor가 생성된 직후 호출되는 상속받은 초기화 함수. 기물의 크기와 위치를 설정
    void Initialize() override;
    // 기물이 공격받을경우 호출
    void BeAttacked();
    // 기물의 색상확인
    bool IsBlack() const { return mColor == PieceColor::Black; }
    // 기물이 선택될 경우 호출됨
    void Selected();
    // 기물이 선택 해제될 경우 호출됨
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
    // 기물의 현재 좌표 (정수 좌표계)
    Coordinates2 mCurrentPosition;
    // 기물의 타입
    PieceType mType;
    // 기물의 색상
    PieceColor mColor;
    // 적군의 색상
    PieceColor mEnemyColor;
    // 기물의 크기 (픽셀 단위)
    size_t mSize;

    // 기물이 다음 이동 가능한 위치
    std::vector<Coordinates2> mNextPosition;
    // 기물의 sprite
    std::weak_ptr<SpriteComponent> mSpriteComponent;

    // 다음 이동위치를 갱신하는 인터페이스 함수. 턴이 시작될때 StartTurn() 함수에 의해서 호출된다.
    virtual void UpdateNextPosition() = 0;
};

class Pawn : public Piece
{
public:
    Pawn(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Pawn();

    // 해당 위치로 이동하는 함수. Piece::MovePieceTo 를 반드시 호출해야함.
    void MovePieceTo(const Coordinates2& nextPosition) override;
    // Turn을 시작할때 호출되는 함수. Piece::StartTurn를 반드시 호출해야함.
    void StartTurn() override;

    //Setter & Getter 
    bool IsMoveTwoSquare() { return mbMoveTwoSquare; }
    
private:
    // 다음 위치를 갱신하는 함수
    void UpdateNextPosition() override;
    // 이번 움직임이 앙파상이라면 true
    bool IsThisMoveEnpassant(const Coordinates2& side);
    // 앙파상 실행
    void DoEnpassant(const Coordinates2& side);
    // 폰의 다음 위치를 갱신하는 helper 함수들
    void UpdateForwardDirection();
    void UpdateDiagonalDirection(const Coordinates2& sideDirection);
    void UpdateEnpassantDirection(const Coordinates2& sideDirection);

    // 폰의 이동 방향(black일경우 + / whtie 일경우 -)
    Coordinates2 mMoveDirection;
    // 한 번 움직이면 true
    bool mbFirstMove;
    // 앙파상 판정을 위해 사용. 두칸 움직였다면 true, 다음 턴 시작될때 false
    bool mbMoveTwoSquare;
    // 이번턴에 앙파상을 시전할 수 있다면 true
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







