#pragma once
#include "Core/Math.h"
#include <vector>
#include <memory>
class Square;
class Piece;

class GameManager
{
public:
    GameManager(class Game& game);
    ~GameManager();
    
    // setter & getter
    Game& GetGame() const { return mGame; }
    Vector2 GetLocationOf(const Coordinates2& position);

private:
    void CreateBoard();

    Game& mGame;
    // ĭ �ϳ��� ũ��(�ȼ�����)
    size_t mSquareSize;
    // �⹰ �ϳ��� ũ��(�ȼ�����)
    size_t mPieceSize;

    std::vector<std::vector<std::shared_ptr<Square>>> mBoard;
    std::vector<std::shared_ptr<Piece>> mBlackPieces;
    std::vector<std::shared_ptr<Piece>> mWhitePieces;
};


