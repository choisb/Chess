#pragma once
#include "Core/Math.h"
#include "PieceTypes.h"
#include "Player.h"
#include <vector>
#include <memory>

class Square;
class Piece;

class GameManager
{
public:
    GameManager(class Game& game);
    ~GameManager();

    // ������ ���� ȣ��Ǵ� �ʱ�ȭ �Լ�
    void Initialize();
    // �Ҹ�Ǳ� ������ ȣ��Ǵ� �Լ�. ���ҽ� ��������
    void Shutdown();
    // ��Ŭ�� �Է� �޾��� �� ȣ��Ǵ� �Լ�. ��Ŭ���� ���콺 ��ġ���� �Ű������� ���� �޴´�.
    void LeftClickDown(const Coordinates2& position);
    // �������Ӹ��� ȣ��Ǵ� UpdateGame �Լ�
    void UpdateGame(float deltaTime) {};
    // Piece �迭���� Piece ����
    void RemovePiece(const std::shared_ptr<Piece>& target, PieceColor color);
    // ���� ���ʷ� �� toggle
    void NextTurn();
    // Square�� ���� ������ �ε������� Ȯ��
    static bool ValidIndex(const Coordinates2& position);
    // Check �߻�
    void BeChecked(PieceColor checkedColor);

    // Setter & Getter
    Game& GetGame() const { return mGame; }
    Vector2 GetActorLocationOf(const Coordinates2& position);
    std::shared_ptr<Square> GetSquare(const Coordinates2& position) { return mBoard[position.y][position.x]; }
    size_t GetPieceSize() const { return mPieceSize; }
    bool IsBlackTurn() const { return mbBlackTurn; }



private:
    // ü�� ���� ����
    void CreateBoard();

    // Game ��������
    Game& mGame;
    // ĭ �ϳ��� ũ��(�ȼ�����)
    size_t mSquareSize;
    // �⹰ �ϳ��� ũ��(�ȼ�����)
    size_t mPieceSize;

    // �θ��� �÷��̾�
    Player mBlackPlayer;
    Player mWhitePlayer;

    // ���� ����
    bool mbBlackTurn;

    // 8*8 Square Board
    std::vector<std::vector<std::shared_ptr<Square>>> mBoard;

};


