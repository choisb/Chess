#include "GameManager.h"
#include "Square.h"
#include "Pawn.h"
#include "Core/Game.h"

GameManager::GameManager(Game& game)
    : mGame(game)
    , mSquareSize(96)
    , mPieceSize(80)
{
    SDL_Log("GameManager()");
    CreateBoard();
}
GameManager::~GameManager()
{
    SDL_Log("~GameManager()");

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
            mBoard[i][j] = GetGame().CreateActor<Square>(GetGame(),Coordinates2(j, i), mSquareSize).lock();
        }
    }

    // 체스 기물 생성
    mBlackPieces.reserve(16);

    for (int i = 0; i < 8; ++i)
    {
        auto piece = GetGame().CreateActor<Pawn>(GetGame(), *this, Color::Black, Coordinates2(i, 1), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }
    for (int i = 0; i < 8; ++i)
    {
        auto piece = GetGame().CreateActor<Pawn>(GetGame(), *this, Color::Black, Coordinates2(i, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    mWhitePieces.reserve(16);
    for (int i = 0; i < 8; ++i)
    {
        auto piece = GetGame().CreateActor<Pawn>(GetGame(), *this, Color::Withe, Coordinates2(i, 6), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }
    for (int i = 0; i < 8; ++i)
    {
        auto piece = GetGame().CreateActor<Pawn>(GetGame(), *this, Color::Withe, Coordinates2(i, 7), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

}
Vector2 GameManager::GetLocationOf(const Coordinates2& position)
{
    return mBoard[position.y][position.x]->GetActorLocation();
}
