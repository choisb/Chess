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
    void Initialize() override;
    void UnOccupied() { mPiece.reset(); };
    void Occupied(const std::shared_ptr<Piece>& piece);
    void Selected();
    void UnSelected();

    // Getter & Setter
    PieceType GetTypeOfPiece() const;
    
private:
    // GameManager 참조변수
    GameManager& mGameManager;
    // Square의 보드 기준 정수 좌표
    Coordinates2 mPosition;
    // Square의 sprite
    std::weak_ptr<SpriteComponent> mNormalSprite;
    // Square가 선택됐을 때 하이라이팅되는 Sprite
    std::weak_ptr<SpriteComponent> mHighlightSprite;
    // Square에 놓여져 있는 기물
    std::weak_ptr<Piece> mPiece;
    // Square의 크기
    size_t mSize;
};


