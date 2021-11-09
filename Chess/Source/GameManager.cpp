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
    // 생성된 모든 기물들의 Next Position을 갱신
    UpdateAllNextPositionOfPiece();
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
Vector2 GameManager::GetActorLocationOf(const Coordinates2& position)
{
    return mBoard[position.y][position.x]->GetActorLocation();
}

void GameManager::UpdateAllNextPositionOfPiece()
{
    mBlackPlayer.UpdateAllNextPositionOfPiece();
    mWhitePlayer.UpdateAllNextPositionOfPiece();
}

void GameManager::LeftClickDown(const Coordinates2& position)
{
    SDL_Log("Left Click (%d, %d)", position.x, position.y);
    const int col = static_cast<int>(position.x / mSquareSize);
    const int row = static_cast<int>(position.y / mSquareSize);

    auto prevSquare = mSelectedSquare.lock();
    // 이전 선택영역과 동일한 경우
    if (prevSquare == mBoard[row][col])
    {
        // 선택 해제
        prevSquare->UnSelected();
    }
    // 이전 선택과 다르고 이전에 선택된 영역이 존재하는 경우
    else if (prevSquare /*&& prevSquare != mBoard[row][col]*/)
    {
        prevSquare->UnSelected();
        mBoard[row][col]->Selected();
        mSelectedSquare = mBoard[row][col];

    }
    // 이전에 선택된 영역이 존재하지 않는경우
    else /* !prevSquare  && prevSquare != mBoard[row][col]*/
    {
        mBoard[row][col]->Selected();
        mSelectedSquare = mBoard[row][col];
    }
}


