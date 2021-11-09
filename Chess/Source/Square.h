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
    // �������� �ʱ�ȭ�� ���� ȣ��Ǵ� �Լ�. actor�� ��ġ�� ũ�� ����
    void Initialize() override;
    // ���� �������� �⹰�� ������ �����Ҷ� ȣ��
    void UnOccupied() { mPiece.reset(); };
    // ���ο� �⹰�� �����Ҷ� ȣ��
    void Occupied(const std::shared_ptr<Piece>& piece);
    // ���� ĭ ���õɶ� ȣ��
    void Selected();
    // ���� ĭ ���� ����ɶ� ȣ��
    void UnSelected();
    // ��ĭ���� Ȯ��
    bool IsEmpty() const { return mPiece.expired(); }
    // ����ĭ�� ���� ��ȯ (������� ��� defalut�� ��ȯ)
    PieceColor GetColor() const;

    // Getter & Setter
    PieceType GetTypeOfPiece() const;
    std::weak_ptr<Piece> GetPiece() const { return mPiece; }
    Coordinates2 GetPosition() const { return mPosition; }
    
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


