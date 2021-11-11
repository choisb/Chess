#pragma once
#include "Core/Actor.h"
#include "Core/Debugging.h"
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
    void Unselected();
    // ���� ĭ�� �ĺ����� ������ �� ȣ��
    void BeCandidate();
    // ���� ĭ�� �ĺ������� ��ҵ� �� ȣ��
    void CancelCandidate();
    // �ش� �������κ��� ���ݹ��� �� ȣ���
    void BeAttackedBy(PieceColor color);
    void ReleaseFromAttackBy(PieceColor color);


    // ��ĭ���� Ȯ��
    bool IsEmpty() const { return mPiece.expired(); }
    // �ĺ������� Ȯ��
    bool IsCandidate() const { return mbCandidate; }
    // ���õ� ��ġ���� Ȯ��
    bool IsSelected() const { return mbSelected; }
    // ����ĭ�� ���� ��ȯ (������� ��� defalut�� ��ȯ)
    PieceColor GetColor() const;
    // ����ĭ�� enemyColor�� ���� �ϰ� �ִ� ���� ��ȯ
    bool IsAttacked(PieceColor enemyColor) const { return (enemyColor == PieceColor::Black) ? mbAttackedByBlack : mbAttackedByWhite; }



    // Getter & Setter
    PieceType GetTypeOfPiece() const;
    std::weak_ptr<Piece> GetPiece() const { return mPiece; }
    Coordinates2 GetPosition() const { return mPosition; }
    
private:
    // GameManager ��������
    GameManager& mGameManager;
    // Square�� ���� ���� ���� ��ǥ
    Coordinates2 mPosition;
    // Square�� ũ��
    size_t mSize;
    // Square�� �ĺ����� ���õȰ�� true
    bool mbCandidate;
    // Square�� ���õ� ��� true
    bool mbSelected;
    // Square�� �ش� �÷��̾�� ���ݹް� �ִ� ��� true
    bool mbAttackedByBlack;
    bool mbAttackedByWhite;

    // Square�� ������ �ִ� �⹰
    std::weak_ptr<Piece> mPiece;
    // Square�� sprite
    std::weak_ptr<SpriteComponent> mNormalSprite;
    // Square�� ���õ��� �� ���̶����õǴ� Sprite
    std::weak_ptr<SpriteComponent> mSeletedSprite;
    // Square�� �ĺ����� ���õ��� �� ���̶����õǴ� Sprite
    std::weak_ptr<SpriteComponent> mCandidatedSprite;
#if DEBUGGING_ATTACK_SPRITE
    // Square�� �� �÷��̾�� ���ݹް� ���� �� Sprite // ������
    std::weak_ptr<SpriteComponent> mAttackedByBlackSprite;
    std::weak_ptr<SpriteComponent> mAttackedByWhiteSprite;
#endif
};


