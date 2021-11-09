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
    // GameManager ��������
    GameManager& mGameManager;
    // Square�� ���� ���� ���� ��ǥ
    Coordinates2 mPosition;
    // Square�� sprite
    std::weak_ptr<SpriteComponent> mNormalSprite;
    // Square�� ���õ��� �� ���̶����õǴ� Sprite
    std::weak_ptr<SpriteComponent> mHighlightSprite;
    // Square�� ������ �ִ� �⹰
    std::weak_ptr<Piece> mPiece;
    // Square�� ũ��
    size_t mSize;
};


