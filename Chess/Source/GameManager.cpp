#include "GameManager.h"
#include "Square.h"
#include "Piece.h"
#include "Core/Game.h"
#include <algorithm>

GameManager::GameManager(Game& game)
    : mGame(game)
    , mSquareSize(96)
    , mPieceSize(96)
{
    CreateBoard();
    CreatePieces();
}
GameManager::~GameManager()
{
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
            mBoard[i][j] = GetGame().CreateActor<Square>(GetGame(),Coordinates2(j, i), mSquareSize).lock();
        }
    }
}
void GameManager::CreatePieces()
{
    // ü�� �⹰ ����
    mBlackPieces.reserve(16);

    // Black Rook
    {
        auto piece = GetGame().CreateActor<Rook>(GetGame(), *this, Color::Black, Coordinates2(0, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Rook>(GetGame(), *this, Color::Black, Coordinates2(7, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    // Black Knight
    {
        auto piece = GetGame().CreateActor<Knight>(GetGame(), *this, Color::Black, Coordinates2(1, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Knight>(GetGame(), *this, Color::Black, Coordinates2(6, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    // Black Bishop
    {
        auto piece = GetGame().CreateActor<Bishop>(GetGame(), *this, Color::Black, Coordinates2(2, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Bishop>(GetGame(), *this, Color::Black, Coordinates2(5, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    // Black Queen
    {
        auto piece = GetGame().CreateActor<Queen>(GetGame(), *this, Color::Black, Coordinates2(4, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    // Black King
    {
        auto piece = GetGame().CreateActor<King>(GetGame(), *this, Color::Black, Coordinates2(3, 0), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }
    // Black Pawn
    for (int i = 0; i < 8; ++i)
    {
        auto piece = GetGame().CreateActor<Pawn>(GetGame(), *this, Color::Black, Coordinates2(i, 1), mPieceSize);
        mBlackPieces.emplace_back(piece.lock());
    }

    mWhitePieces.reserve(16);
    // White Rook
    {
        auto piece = GetGame().CreateActor<Rook>(GetGame(), *this, Color::White, Coordinates2(0, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Rook>(GetGame(), *this, Color::White, Coordinates2(7, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }

    // White Knight
    {
        auto piece = GetGame().CreateActor<Knight>(GetGame(), *this, Color::White, Coordinates2(1, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Knight>(GetGame(), *this, Color::White, Coordinates2(6, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }

    // White Bishop
    {
        auto piece = GetGame().CreateActor<Bishop>(GetGame(), *this, Color::White, Coordinates2(2, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
        piece = GetGame().CreateActor<Bishop>(GetGame(), *this, Color::White, Coordinates2(5, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }

    // White Queen
    {
        auto piece = GetGame().CreateActor<Queen>(GetGame(), *this, Color::White, Coordinates2(4, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }

    // White King
    {
        auto piece = GetGame().CreateActor<King>(GetGame(), *this, Color::White, Coordinates2(3, 7), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }
    // White Pawn
    for (int i = 0; i < 8; ++i)
    {
        auto piece = GetGame().CreateActor<Pawn>(GetGame(), *this, Color::White, Coordinates2(i, 6), mPieceSize);
        mWhitePieces.emplace_back(piece.lock());
    }
}


void GameManager::UpdateGame(float deltaTime)
{
    
}

void GameManager::RemovePiece(const std::shared_ptr<Piece>& target, Color color)
{
    std::vector<std::shared_ptr<Piece>>* pieces;
    if (color == Color::Black)
        pieces = &mBlackPieces;
    else
        pieces = &mWhitePieces;

    // Ž�� �� swap - pop ������� ����
    auto iter = std::find(pieces->begin(), pieces->end(), target);
    std::iter_swap(iter, std::prev(pieces->end()));
    pieces->pop_back();    
}
Vector2 GameManager::GetLocationOf(const Coordinates2& position)
{
    return mBoard[position.y][position.x]->GetActorLocation();
}
std::weak_ptr<Square> GameManager::GetSquare(const Coordinates2 & position)
{
    return std::weak_ptr<Square>(mBoard[position.y][position.x]);
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

