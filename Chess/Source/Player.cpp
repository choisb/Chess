#include "Player.h"
#include "Piece.h"
#include "GameManager.h"
#include "Core/Game.h"
#include "Square.h"

Player::Player(GameManager& gameManager, PieceColor color)
    : mGameManager(gameManager)
    , mColor(color)
    , mbIsCheck(false)
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
    // 체스 기물 생성
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
    mPieces.emplace_back(game.CreateActor<Queen>(game, mGameManager, mColor, Coordinates2(3, majorRow), pieceSize));
    
    // King
    mPieces.emplace_back(game.CreateActor<King>(game, mGameManager, mColor, Coordinates2(4, majorRow), pieceSize));

    // Pawn
    for (int i = 0; i < 8; ++i)
        mPieces.emplace_back(game.CreateActor<Pawn>(game, mGameManager, mColor, Coordinates2(i, pawnRow), pieceSize));

}

void Player::RemovePiece(const std::shared_ptr<Piece>& target)
{
    // 탐색 후 swap - pop 방식으로 제거
    auto iter = std::find(mPieces.begin(), mPieces.end(), target);
    std::iter_swap(iter, std::prev(mPieces.end()));
    mPieces.pop_back();
}


void Player::LeftClickDown(const std::shared_ptr<Square>& square)
{
    // 첫 번째 기물 선택
    if (mSelectedPiece.expired() && mColor == square->GetColor())
    {
        SelectPieceForMove(square->GetPiece().lock());
    }
    // 선택된 기물이 있는 경우 이동할 위치 선택
    else if (!mSelectedPiece.expired())
    {
        SeletSquareToMove(square);
    }
}

void Player::StartTurn()
{
    // 모든 기물들에게 turn이 시작되었음을 알림. (각 함수들은 별도의 반복문에서 회출됨)
    for (auto& piece : mPieces)
    {
        piece->StartTurn(); // 폰의 앙파상을 위해서 사용됨
    }
    // 현재 공격중이 모든 지역을 공격 해제, 이동 가능 지역 해제
    for (auto& piece : mPieces)
    {
        piece->ReleaseFromAllAttacks();
        piece->ReleaseMoveLocation();
    }
    // 새롭게 공격 지역 탐색
    for (auto& piece : mPieces)
    {
        piece->SearchAttackAndMoveLocation();
    }
}

void Player::EndTurn()
{
    // 현재 공격중이 모든 지역을 공격 해제, 이동 가능 지역 해제
    for (auto& piece : mPieces)
    {
        piece->ReleaseFromAllAttacks();
        piece->ReleaseMoveLocation();
    }
    // 새롭게 공격 지역 및 이동 지역 탐색
    for (auto& piece : mPieces)
    {
        piece->SearchAttackAndMoveLocation();
    }
}

void Player::BeCheck()
{
    mbIsCheck = true;
}

void Player::ReleaseCheck()
{
    mbIsCheck = false;
}

void Player::SelectPieceForMove(const std::shared_ptr<Piece>& piece)
{
    piece->Selected();
    mSelectedPiece = piece;
}

void Player::SeletSquareToMove(const std::shared_ptr<Square>& square)
{
    // 선택한 위치가 후보지라면 이동
    if (square->IsCandidate())
    {
        // 이동 후 턴 종료
        mSelectedPiece.lock()->MovePieceTo(square->GetPosition());
        mSelectedPiece.reset();
        mGameManager.NextTurn();
    }
    // 선택한 위치가 현재 선택된 위치와 동일하다면 선택 취소
    else if (square->IsSelected())
    {
        mSelectedPiece.lock()->Unselected();
        mSelectedPiece.reset();
    }
    // 선택한 위치가 자신의 다른 기물이라면 선택 변경
    else if (square->GetColor() == mColor)
    {
        mSelectedPiece.lock()->Unselected();
        mSelectedPiece.reset();
        SelectPieceForMove(square->GetPiece().lock());
    }
    // 그 외에는 아무 동작도 하지 않음
    else
        ;

}

