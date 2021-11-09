#pragma once
#include "PieceTypes.h"
#include <vector>
#include <memory>

class GameManager;
class Piece;

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

private:
    // Game Manager ���� ����
    GameManager& mGameManager;
    // �÷��̾� ��������
    PieceColor mColor;

    // �⹰�� �迭
    std::vector<std::shared_ptr<Piece>> mPieces;
};

