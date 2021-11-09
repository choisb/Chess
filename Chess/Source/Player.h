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
    // �迭�� �ִ� ��� �⹰���� ���� �̵����� ��ġ�� ���.
    // �Ϲ������� �� ������ �ϳ��� �⹰�̶� �̵��� ��� ��� ������.
    // GameManager::UpdateAllNextPositionOfPiece()���� ȣ���
    void UpdateAllNextPositionOfPiece();

    void LeftClickDown(const std::shared_ptr<Square>& square);
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

};

