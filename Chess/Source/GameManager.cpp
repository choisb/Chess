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
    // ������ ��� �⹰���� Next Position�� ����
    UpdateAllNextPositionOfPiece();
}
void GameManager::CreateBoard()
{
    // ���� ���� (8*8 Square ����)
    mBoard.resize(8);
    for (int i = 0; i < 8; ++i)
    {
        mBoard[i].resize(8);
        for (int j = 0; j < 8; ++j)
        {
            // Square ������ ȣ��
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
    // ���� ���ÿ����� ������ ���
    if (prevSquare == mBoard[row][col])
    {
        // ���� ����
        prevSquare->UnSelected();
    }
    // ���� ���ð� �ٸ��� ������ ���õ� ������ �����ϴ� ���
    else if (prevSquare /*&& prevSquare != mBoard[row][col]*/)
    {
        prevSquare->UnSelected();
        mBoard[row][col]->Selected();
        mSelectedSquare = mBoard[row][col];

    }
    // ������ ���õ� ������ �������� �ʴ°��
    else /* !prevSquare  && prevSquare != mBoard[row][col]*/
    {
        mBoard[row][col]->Selected();
        mSelectedSquare = mBoard[row][col];
    }
}


