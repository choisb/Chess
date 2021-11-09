#pragma once
#include "Core/Math.h"
#include "PieceTypes.h"
#include "Player.h"
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
    void LeftClickDown(const Coordinates2& position);
    // 매프레임마다 호출되는 UpdateGame 함수
    void UpdateGame(float deltaTime) {};
    // Piece 배열에서 Piece 삭제
    void RemovePiece(const std::shared_ptr<Piece>& target, PieceColor color);
    // 모든 체스 기물들의 NextPosition 갱신. 하나의 기물이 이동할 경우 호출됨
    void UpdateAllNextPositionOfPiece();
    void Shutdown();

    void ResetSelectedSquare() { mSelectedSquare.reset(); }
    void SetSelectedSquare(std::shared_ptr<Square> square) { mSelectedSquare = std::move(square); }

    // setter & getter
    Game& GetGame() const { return mGame; }
    Vector2 GetActorLocationOf(const Coordinates2& position);
    std::shared_ptr<Square> GetSquare(const Coordinates2& position) { return mBoard[position.y][position.x]; }
    size_t GetPieceSize() const { return mPieceSize; }

private:
    // 체스 보드 생성
    void CreateBoard();

    // Game 참조변수
    Game& mGame;
    // 칸 하나의 크기(픽셀단위)
    size_t mSquareSize;
    // 기물 하나의 크기(픽셀단위)
    size_t mPieceSize;

    // 두명의 플레이어
    Player mBlackPlayer;
    Player mWhitePlayer;

    // 선택된 Square
    std::weak_ptr<Square> mSelectedSquare;

    // 8*8 Square Board
    std::vector<std::vector<std::shared_ptr<Square>>> mBoard;

};


