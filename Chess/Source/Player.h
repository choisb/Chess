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
    // ���� ���� ������ ȣ��Ǵ� �Լ�. ���ҽ� ����
    void Shutdown();
    // Game Manager�� �����ڿ��� ȣ��Ǵ� �Լ�. ��� �⹰���� ������
    void CreatePieces();
    // Pieces �迭���� �⹰ �ϳ��� �����ϴ� �Լ�. ���� �⹰�� ������ �� ���
    void RemovePiece(const std::shared_ptr<Piece>& target);
    // �ش� �÷��̾��� ���ʿ� ��Ŭ���� ������ ��� ȣ���
    void LeftClickDown(const std::shared_ptr<Square>& square);
    // �÷��̾��� ���ʰ� ���۵� ��� ȣ���. ���ʰ� ���۵ǰ� �ʿ��� �ʱ�ȭ ����� ȣ���� �� ����.
    void StartTurn();
    // �÷��̾� ���ʰ� ����ɶ� ȣ���. ���ݰ��� ��ġ�� �̵����� ��ġ ����
    void EndTurn();
    // ŷ�� üũ�ɰ�� ȣ��
    void BeChecked();
    // ŷ�� üũ���� ������ ��� ȣ��
    void ReleaseCheck();

    // Setter & Getter
    bool IsCheck() const { return mbIsCheck; }
private:
    // ù��° �⹰ ����
    void SelectPieceForMove(const std::shared_ptr<Piece>& piece);
    // �ι�° �̵���ġ ����
    void SeletSquareToMove(const std::shared_ptr<Square>& square);

    // Game Manager ���� ����
    GameManager& mGameManager;
    // �÷��̾� ��������
    PieceColor mColor;
    // ���� ���õ� �⹰
    std::weak_ptr<Piece> mSelectedPiece;
    // �⹰�� �迭
    std::vector<std::shared_ptr<Piece>> mPieces;
    // üũ ����
    bool mbIsCheck;

};

