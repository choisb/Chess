#pragma once
#include "Core/Math.h"
#include "PieceTypes.h"
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
    // �������Ӹ��� ȣ��Ǵ� UpdateGame �Լ�
    virtual void UpdateGame(float deltaTime);
    // Piece �迭���� Piece ����
    void RemovePiece(const std::shared_ptr<Piece>& target, Color color);
    // ��� ü�� �⹰���� NextPosition ����. �ϳ��� �⹰�� �̵��� ��� ȣ���
    void UpdateAllNextPositionOfPiece();


    // setter & getter
    Game& GetGame() const { return mGame; }
    Vector2 GetLocationOf(const Coordinates2& position);
    std::weak_ptr<Square> GetSquare(const Coordinates2& position);


private:
    // ü�� ���� ����
    void CreateBoard();
    // ü�� �⹰ ����
    void CreatePieces();
    Game& mGame;
    // ĭ �ϳ��� ũ��(�ȼ�����)
    size_t mSquareSize;
    // �⹰ �ϳ��� ũ��(�ȼ�����)
    size_t mPieceSize;

    std::vector<std::vector<std::shared_ptr<Square>>> mBoard;
    std::vector<std::shared_ptr<Piece>> mBlackPieces;
    std::vector<std::shared_ptr<Piece>> mWhitePieces;
};


