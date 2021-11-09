#pragma once
#include "Core/Math.h"
#include "PieceTypes.h"
#include <vector>
#include <memory>
class Square;
class Piece;

class GameManager
{
public:
    GameManager(class Game& game);
    ~GameManager();

    // 생성자 직후 호출되는 초기화 함수
    void Initialize();
    // 매프레임마다 호출되는 UpdateGame 함수
    virtual void UpdateGame(float deltaTime);
    // Piece 배열에서 Piece 삭제
    void RemovePiece(const std::shared_ptr<Piece>& target, Color color);
    // 모든 체스 기물들의 NextPosition 갱신. 하나의 기물이 이동할 경우 호출됨
    void UpdateAllNextPositionOfPiece();


    // setter & getter
    Game& GetGame() const { return mGame; }
    Vector2 GetLocationOf(const Coordinates2& position);
    std::weak_ptr<Square> GetSquare(const Coordinates2& position);


private:
    // 체스 보드 생성
    void CreateBoard();
    // 체스 기물 생성
    void CreatePieces();
    Game& mGame;
    // 칸 하나의 크기(픽셀단위)
    size_t mSquareSize;
    // 기물 하나의 크기(픽셀단위)
    size_t mPieceSize;

    std::vector<std::vector<std::shared_ptr<Square>>> mBoard;
    std::vector<std::shared_ptr<Piece>> mBlackPieces;
    std::vector<std::shared_ptr<Piece>> mWhitePieces;
};


