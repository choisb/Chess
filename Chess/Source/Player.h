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
    // 해당 플레이어의 차례에 좌클릭이 눌렸을 경우 호출됨
    void LeftClickDown(const std::shared_ptr<Square>& square);
    // 플레이어의 차례가 시작될 경우 호출됨. 차례가 시작되고 필요한 초기화 기능을 호출할 수 있음.
    void StartTurn();
    // 플레이어 차례가 종료될때 호출됨. 공격가능 위치와 이동가능 위치 재계산
    void EndTurn();
    // 킹이 체크될경우 호출
    void BeChecked();
    // 킹이 체크에서 해제될 경우 호출
    void ReleaseCheck();

    // Setter & Getter
    bool IsCheck() const { return mbIsCheck; }
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
    // 체크 상태
    bool mbIsCheck;

};

