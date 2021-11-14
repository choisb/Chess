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
    // 소멸되기 직전에 호출되는 함수. 리소스 정리역할
    void Shutdown();
    // 좌클릭 입력 받았을 때 호출되는 함수. 좌클릭시 마우스 위치값을 매개변수로 전달 받는다.
    void LeftClickDown(const Coordinates2& position);
    // 매프레임마다 호출되는 UpdateGame 함수
    void UpdateGame(float deltaTime) {};
    // Piece 배열에서 Piece 삭제
    void RemovePiece(const std::shared_ptr<Piece>& target, PieceColor color);
    // 다음 차례로 턴 toggle
    void NextTurn();
    // Square에 접근 가능한 인덱스인지 확인
    static bool ValidIndex(const Coordinates2& position);
    // Check 발생
    void BeChecked(PieceColor checkedColor);

    // Setter & Getter
    Game& GetGame() const { return mGame; }
    Vector2 GetActorLocationOf(const Coordinates2& position);
    std::shared_ptr<Square> GetSquare(const Coordinates2& position) { return mBoard[position.y][position.x]; }
    size_t GetPieceSize() const { return mPieceSize; }
    bool IsBlackTurn() const { return mbBlackTurn; }



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

    // 게임 순서
    bool mbBlackTurn;

    // 8*8 Square Board
    std::vector<std::vector<std::shared_ptr<Square>>> mBoard;

};


