#include "GameManager.h"
#include "Square.h"
#include "Piece.h"
#include "Core/Game.h"
#include <algorithm>

GameManager::GameManager(Game& game)
    : mGame(game)
    , mSquareSize(96)
    , mPieceSize(64)
    , mBlackPlayer(*this, PieceColor::Black)
    , mWhitePlayer(*this, PieceColor::White)
    , mbBlackTurn(false)
{
    CreateBoard();
    mBlackPlayer.CreatePieces();
    mWhitePlayer.CreatePieces();
}
GameManager::~GameManager()
{
}
void GameManager::Shutdown()
{
    mBoard.clear();
    mBlackPlayer.Shutdown();
    mWhitePlayer.Shutdown();
}
void GameManager::Initialize()
{
    // White player부터 게임 시작
    mbBlackTurn = false;
    mBlackPlayer.EndTurn();
    mWhitePlayer.StartTurn();
}
void GameManager::CreateBoard()
{
    // 보드 생성 (8*8 Square 생성)
    mBoard.resize(8);
    for (int i = 0; i < 8; ++i)
    {
        mBoard[i].resize(8);
        for (int j = 0; j < 8; ++j)
        {
            // Square 생성자 호출
            mBoard[i][j] = GetGame().CreateActor<Square>(GetGame(),*this, Coordinates2(j, i), mSquareSize);
        }
    }
}

void GameManager::RemovePiece(const std::shared_ptr<Piece>& target, PieceColor color)
{
    if (color == PieceColor::Black)
        mBlackPlayer.RemovePiece(target);
    else
        mWhitePlayer.RemovePiece(target);
}
bool GameManager::ValidIndex(const Coordinates2 & position)
{
    return (position.x >= 0 && position.x < 8 && position.y >= 0 && position.y < 8);
}
Vector2 GameManager::GetActorLocationOf(const Coordinates2& position)
{
    return mBoard[position.y][position.x]->GetActorLocation();
}

void GameManager::NextTurn()
{
    // 게임 순서 toggle
    mbBlackTurn = !mbBlackTurn;
    if (mbBlackTurn)
    {
        mWhitePlayer.EndTurn();
        mBlackPlayer.StartTurn();
    }
    else
    {
        mBlackPlayer.EndTurn();
        mWhitePlayer.StartTurn();
    }
}

void GameManager::LeftClickDown(const Coordinates2& position)
{
    //SDL_Log("Left Click (%d, %d)", position.x, position.y);
    const int col = static_cast<int>(position.x / mSquareSize);
    const int row = static_cast<int>(position.y / mSquareSize);

    SDL_Log("selecte square: (%d, %d) / Color: %d / Type: %d", col, row, mBoard[row][col]->GetColor(), mBoard[row][col]->GetTypeOfPiece());
    // 클릭된 square를 현재 턴인 player에게 전달
    if (mbBlackTurn)
        mBlackPlayer.LeftClickDown(mBoard[row][col]);
    else
        mWhitePlayer.LeftClickDown(mBoard[row][col]);
}


