#include "GameManager.h"
#include "Square.h"
#include "Piece.h"
#include "Core/Game.h"
#include <algorithm>

GameManager::GameManager(Game& game)
    : mGame(game)
    , mSquareSize(96)
    , mPieceSize(64)
{
    CreateBoard();
    CreatePieces();
}
GameManager::~GameManager()
{
}
void GameManager::Shutdown()
{
    mBoard.clear();
    mBlackPieces.clear();
    mWhitePieces.clear();
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
            mBoard[i][j] = GetGame().CreateActor<Square>(GetGame(),*this, Coordinates2(j, i), mSquareSize).lock();
        }
    }
}
void GameManager::CreatePieces()
{
    // ü�� �⹰ ����
    mBlackPieces.reserve(16);

    // Black Rook
    {
        auto piece = GetGame().CreateActor<Rook>(GetGame(), *this, PieceColor::Black, Coordinates2(0, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Rook>(GetGame(), *this, PieceColor::Black, Coordinates2(7, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    // Black Knight
    {
        auto piece = GetGame().CreateActor<Knight>(GetGame(), *this, PieceColor::Black, Coordinates2(1, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Knight>(GetGame(), *this, PieceColor::Black, Coordinates2(6, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    // Black Bishop
    {
        auto piece = GetGame().CreateActor<Bishop>(GetGame(), *this, PieceColor::Black, Coordinates2(2, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Bishop>(GetGame(), *this, PieceColor::Black, Coordinates2(5, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    // Black Queen
    {
        auto piece = GetGame().CreateActor<Queen>(GetGame(), *this, PieceColor::Black, Coordinates2(4, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    // Black King
    {
        auto piece = GetGame().CreateActor<King>(GetGame(), *this, PieceColor::Black, Coordinates2(3, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }
    // Black Pawn
    for (int i = 0; i < 8; ++i)
    {
        auto piece = GetGame().CreateActor<Pawn>(GetGame(), *this, PieceColor::Black, Coordinates2(i, 1), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    mWhitePieces.reserve(16);
    // White Rook
    {
        auto piece = GetGame().CreateActor<Rook>(GetGame(), *this, PieceColor::White, Coordinates2(0, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Rook>(GetGame(), *this, PieceColor::White, Coordinates2(7, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }

    // White Knight
    {
        auto piece = GetGame().CreateActor<Knight>(GetGame(), *this, PieceColor::White, Coordinates2(1, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Knight>(GetGame(), *this, PieceColor::White, Coordinates2(6, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }

    // White Bishop
    {
        auto piece = GetGame().CreateActor<Bishop>(GetGame(), *this, PieceColor::White, Coordinates2(2, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Bishop>(GetGame(), *this, PieceColor::White, Coordinates2(5, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }

    // White Queen
    {
        auto piece = GetGame().CreateActor<Queen>(GetGame(), *this, PieceColor::White, Coordinates2(4, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }

    // White King
    {
        auto piece = GetGame().CreateActor<King>(GetGame(), *this, PieceColor::White, Coordinates2(3, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }
    // White Pawn
    for (int i = 0; i < 8; ++i)
    {
        auto piece = GetGame().CreateActor<Pawn>(GetGame(), *this, PieceColor::White, Coordinates2(i, 6), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }
}


void GameManager::UpdateGame(float deltaTime)
{
    
}

void GameManager::RemovePiece(const std::shared_ptr<Piece>& target, PieceColor color)
{
    std::vector<std::shared_ptr<Piece>>* pieces;
    if (color == PieceColor::Black)
        pieces = &mBlackPieces;
    else
        pieces = &mWhitePieces;

    // Ž�� �� swap - pop ������� ����
    auto iter = std::find(pieces->begin(), pieces->end(), target);
    std::iter_swap(iter, std::prev(pieces->end()));
    pieces->pop_back();    
}
Vector2 GameManager::GetActorLocationOf(const Coordinates2& position)
{
    return mBoard[position.y][position.x]->GetActorLocation();
}

void GameManager::UpdateAllNextPositionOfPiece()
{
    for (auto& piece : mBlackPieces)
    {
        piece->UpdateNextPosition();
    }
    for (auto& piece : mWhitePieces)
    {
        piece->UpdateNextPosition();
    }
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


