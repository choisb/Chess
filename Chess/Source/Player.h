#pragma once
#include "PieceTypes.h"
#include <vector>
#include <memory>

class GameManager;
class Piece;
class Square;
class Player
{
public:
    Player(GameManager& gameManager, PieceColor color);
    ~Player();
    // 게임 종료 직전에 호출되는 함수. 리소스 정리
    void Shutdown();
    // Game Manager의 생성자에서 호출되는 함수. 모든 기물들을 생성함
    void CreatePieces();
    // Pieces 배열에서 기물 하나를 제거하는 함수. 죽은 기물을 정리할 때 사용
    void RemovePiece(const std::shared_ptr<Piece>& target);
    // 배열에 있는 모든 기물들의 다음 이동가능 위치를 계산.
    // 일반적으로 맵 위에서 하나의 기물이라도 이동할 경우 모두 재계산함.
    // GameManager::UpdateAllNextPositionOfPiece()에서 호출됨
    void UpdateAllNextPositionOfPiece();

    void LeftClickDown(const std::shared_ptr<Square>& square);
private:
    // 첫번째 기물 선택
    void SelectPieceForMove(const std::shared_ptr<Piece>& piece);
    // 두번째 이동위치 선택
    void SeletSquareToMove(const std::shared_ptr<Square>& square);

    // Game Manager 참조 변수
    GameManager& mGameManager;
    // 플레이어 색상정보
    PieceColor mColor;
    // 현재 선택된 기물
    std::weak_ptr<Piece> mSelectedPiece;
    // 기물들 배열
    std::vector<std::shared_ptr<Piece>> mPieces;

};

