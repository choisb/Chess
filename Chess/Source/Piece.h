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

    // Actor가 생성된 직후 호출되는 상속받은 초기화 함수. 기물의 크기와 위치를 설정
    void Initialize() override;
    // 기물을 입력받은 정수 좌표로 이동하는 함수 (bIsUpdateAllNextPosition == false 일 경우 이동 후 갱신 안함)
    virtual void MovePieceTo(const Coordinates2& nextPosition);
    // 턴이 시작될 때 호출되는 가상함수
    virtual void StartTurn() {}
    // 기물이 공격받을경우 호출
    void IsDestroyed();

    // 기물이 선택될 경우 호출됨 (폰을 위한 가상화)
    void Selected();
    // 기물이 선택 해제될 경우 호출됨 (폰을 위한 가상화)
    void Unselected();

    // 해당 지역을 공격한다.
    void AddAttackLocation(const Coordinates2& position);
    // 폰의 mOnlyMoveLocation 초기화를 위해 가상화
    void ReleaseFromAllAttacks();

    // 폰의 이동 배열에 추가하는 함수
    void AddMoveLocation(const Coordinates2& position);
    // mMoveLocation 배열 초기화
    void ReleaseMoveLocation();


    // 공격 al가능 지역을 모두 탐색
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
    // 첫 움직임만 true, 한번 움직인적 있다면 false
    bool mbFirstMove;
    // 기물의 sprite
    std::weak_ptr<SpriteComponent> mSpriteComponent;

    // 다음 이동위치를 탐색하는 인터페이스 함수. 턴이 시작될때 StartTurn() 함수에 의해서 호출된다.
    void SearchInTheDirection(const Coordinates2& direction);
    // 해당 위치로 이동 가능한지, 체크가 발생하지 않는지 확인하여 true / false 반환
    bool CanMoveTo(const Coordinates2& position);

    static const std::array<Coordinates2, 4> StraightDirections;
    static const std::array<Coordinates2, 4> DiagonalDirections;

private:
    // 기물이 공격중인 위치 인터페이스로만 접근해야함.
    std::vector<Coordinates2> mLocationBeingAttacked;
    // 공격은 움직일 수 있는 위치 인터페이스로만 접근해야함.
    std::vector<Coordinates2> mMoveLocation;
};

class Pawn : public Piece
{
public:
    Pawn(class Game& game, class GameManager& gameManager, PieceColor inColor, Coordinates2 inPosition, size_t size = 128);
    ~Pawn();

    // 해당 위치로 이동하는 함수. Piece::MovePieceTo 를 반드시 호출해야함.
    void MovePieceTo(const Coordinates2& nextPosition) override;
    // 턴이 시작하고 호출되는 가상함수
    void StartTurn() override;
    //Setter & Getter 
    bool IsMoveTwoSquare() { return mbMoveTwoSquare; }
    // 다음 위치를 탐색하는 함수
    void SearchAttackAndMoveLocation() override;

private:

    // 이번 움직임이 앙파상이라면 true
    bool IsThisMovementEnpassant(const Coordinates2& side);
    // 앙파상 실행
    void DoEnpassant(const Coordinates2& side);
    // 폰의 다음 위치를 탐색하는 helper 함수들
    void SearchForwardDirection();
    void SearchDiagonalDirection(const Coordinates2& direction);
    void SearchEnpassantDirection(const Coordinates2& direction);

    // 폰의 이동 방향(black일경우 + / whtie 일경우 -)
    Coordinates2 mMoveDirection;
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
    // TODO: 캐슬링 구현
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
    // 인접 위치 탐색
    void SearchAdjacent(const Coordinates2& direction);
    // 캐슬링 탐색
    void SearchCastlingKingSide();
    void SearchCastlingQueenSide();
    // 캐슬링 실행
    void DoCastlingKingSide();
    void DoCastlingQueenSide();
};







