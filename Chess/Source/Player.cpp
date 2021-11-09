#include "Player.h"
#include "Piece.h"
#include "GameManager.h"
#include "Core/Game.h"
#include "Square.h"

Player::Player(GameManager& gameManager, PieceColor color)
    : mGameManager(gameManager)
    , mColor(color)
{

}

Player::~Player()
{

}
void Player::Shutdown()
{
    mPieces.clear();
}
void Player::CreatePieces()
{
    // ü�� �⹰ ����
    mPieces.reserve(16);

    Game& game = mGameManager.GetGame();
    const size_t pieceSize = mGameManager.GetPieceSize();
    int pawnRow{1}, majorRow{0};
    if (mColor == PieceColor::White)
    {
        pawnRow = 6;
        majorRow = 7;
    }

    // Rook
    mPieces.emplace_back(game.CreateActor<Rook>(game, mGameManager, mColor, Coordinates2(0, majorRow), pieceSize));
    mPieces.emplace_back(game.CreateActor<Rook>(game, mGameManager, mColor, Coordinates2(7, majorRow), pieceSize));

    // Knight
    mPieces.emplace_back(game.CreateActor<Knight>(game, mGameManager, mColor, Coordinates2(1, majorRow), pieceSize));
    mPieces.emplace_back(game.CreateActor<Knight>(game, mGameManager, mColor, Coordinates2(6, majorRow), pieceSize));

    // Bishop
    mPieces.emplace_back(game.CreateActor<Bishop>(game, mGameManager, mColor, Coordinates2(2, majorRow), pieceSize));
    mPieces.emplace_back(game.CreateActor<Bishop>(game, mGameManager, mColor, Coordinates2(5, majorRow), pieceSize));

    // Queen
    mPieces.emplace_back(game.CreateActor<Queen>(game, mGameManager, mColor, Coordinates2(4, majorRow), pieceSize));
    
    // King
    mPieces.emplace_back(game.CreateActor<King>(game, mGameManager, mColor, Coordinates2(3, majorRow), pieceSize));

    // Pawn
    for (int i = 0; i < 8; ++i)
        mPieces.emplace_back(game.CreateActor<Pawn>(game, mGameManager, mColor, Coordinates2(i, pawnRow), pieceSize));

}

void Player::RemovePiece(const std::shared_ptr<Piece>& target)
{
    // Ž�� �� swap - pop ������� ����
    auto iter = std::find(mPieces.begin(), mPieces.end(), target);
    std::iter_swap(iter, std::prev(mPieces.end()));
    mPieces.pop_back();
}

void Player::UpdateAllNextPositionOfPiece()
{
}

void Player::LeftClickDown(const std::shared_ptr<Square>& square)
{
    SDL_Log("selected color: %d", static_cast<int>(square->GetColor()));
    // ù ��° �⹰ ����
    if (mSelectedPiece.expired() && mColor == square->GetColor())
    {
        SelectPieceForMove(square->GetPiece().lock());
    }
    // ���õ� �⹰�� �ִ� ��� �̵��� ��ġ ����
    else if (!mSelectedPiece.expired())
    {
        SeletSquareToMove(square);
    }
}

void Player::SelectPieceForMove(const std::shared_ptr<Piece>& piece)
{
    auto square = mGameManager.GetSquare(piece->GetPosition());
    square->Selected();
    mSelectedPiece = piece;
    // TODO: �̵����� ���� ��ŷ
}

void Player::SeletSquareToMove(const std::shared_ptr<Square>& square)
{
    // TODO: �̵� ������ �������� ���� �ڵ� �ʿ� �켱�� ������ �̵����� ����
    // ���� �������� UnSelecte
    auto prevSquare = mGameManager.GetSquare(mSelectedPiece.lock()->GetPosition());
    prevSquare->UnSelected();

    // �̵� �� �� ����
    mSelectedPiece.lock()->MovePieceTo(square->GetPosition());
    mSelectedPiece.reset();
    mGameManager.NextTurn();

}

