#pragma once
#include "Core/Actor.h"
#include <memory>

class SpriteComponent;
class Piece;
class Game;
class GameManager;

class Square : public Actor
{
public:
    Square(Game& game, GameManager& gameManager, Coordinates2 inCoordinates2, size_t size = 128);
    // 생성직후 초기화를 위해 호출되는 함수. actor의 위치와 크기 설정
    void Initialize() override;
    // 현재 점유중인 기물이 점유를 종료할때 호출
    void UnOccupied() { mPiece.reset(); };
    // 새로운 기물이 점유할때 호출
    void Occupied(const std::shared_ptr<Piece>& piece);
    // 현재 칸 선택될때 호출
    void Selected();
    // 현재 칸 선택 종료될때 호출
    void Unselected();
    // 현재 칸이 후보지로 선정될 때 호출
    void BeCandidate();
    // 현재 칸이 후보지에서 취소될 때 호출
    void CancelCandidate();

    // 빈칸인지 확인
    bool IsEmpty() const { return mPiece.expired(); }
    // 후보지인지 확인
    bool IsCandidate() const { return mbCandidate; }
    // 선택된 위치인지 확인
    bool IsSelected() const { return mbSelected; }
    // 현재칸의 색상 반환 (비어있을 경우 defalut값 반환)
    PieceColor GetColor() const;

    // Getter & Setter
    PieceType GetTypeOfPiece() const;
    std::weak_ptr<Piece> GetPiece() const { return mPiece; }
    Coordinates2 GetPosition() const { return mPosition; }
    
private:
    // GameManager 참조변수
    GameManager& mGameManager;
    // Square의 보드 기준 정수 좌표
    Coordinates2 mPosition;
    // Square의 크기
    size_t mSize;
    // Square가 후보지로 선택된경우 true
    bool mbCandidate;
    // Square가 선택된 경우 true
    bool mbSelected;

    // Square에 놓여져 있는 기물
    std::weak_ptr<Piece> mPiece;
    // Square의 sprite
    std::weak_ptr<SpriteComponent> mNormalSprite;
    // Square가 선택됐을 때 하이라이팅되는 Sprite
    std::weak_ptr<SpriteComponent> mSeletedSprite;
    // Square가 후보지로 선택됐을 때 하이라이팅되는 Sprite
    std::weak_ptr<SpriteComponent> mCandidatedSprite;
};


