#include "Piece.h"
#include "PieceTypes.h"
#include "Square.h"
#include "GameManager.h"
#include "Core/Game.h"
#include "Core/SpriteComponent.h"
#include "Core/Renderer.h"

Square::Square(Game& game, GameManager& gameManager, Coordinates2 inCoordinates2, size_t size /* = 128*/)
    : Actor(game)
    , mGameManager(gameManager)
    , mPosition(std::move(inCoordinates2))
    , mSize(size)
    , mbCandidate(false)
    , mbSelected(false)
    , mbAttackedByBlack(false)
    , mbAttackedByWhite(false)
{
    // size가 0보다 작거나 같을 경우 defalut 값으로 초기화
    if (size <= 0) mSize = 128;

    // 체스말 그리는 우선순위가 70
    // NormalSprite 그리는 우선순위 50
    mNormalSprite = CreateComponent<SpriteComponent>(*this,50);

    auto sc = mNormalSprite.lock();
    if ((mPosition.x + mPosition.y) % 2 == 0)
        sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/square brown light_png_shadow_128px.png"));
    else
        sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/square brown dark_png_shadow_128px.png"));

    // Highlight Sprite 그리는 우선순위는 60
    mSeletedSprite = CreateComponent<SpriteComponent>(*this, 60);
    sc = mSeletedSprite.lock();
    sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/selected_square_128px.png"));
    sc->SetDisable();

    // Candidate Sprite 그리는 우선순위는 90
    mCandidatedSprite = CreateComponent<SpriteComponent>(*this, 90);
    sc = mCandidatedSprite.lock();
    sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/candidate_square_128px.png"));
    sc->SetDisable();

#if DEBUGGING_ATTACK_SPRITE
    mAttackedByBlackSprite = CreateComponent<SpriteComponent>(*this, 80);
    sc = mAttackedByBlackSprite.lock();
    sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/attacked_by_black_128px.png"));
    sc->SetDisable();

    mAttackedByWhiteSprite = CreateComponent<SpriteComponent>(*this, 80);
    sc = mAttackedByWhiteSprite.lock();
    sc->SetTexture(GetGame().GetRenderer()->GetTexture("../Chess/Assets/Imgs/attacked_by_white_128px.png"));
    sc->SetDisable();
#endif
}
void Square::Initialize()
{
    //엑터의 크기와 위치 설정
    auto sc = mNormalSprite.lock();
    const float textureSize = static_cast<float>(sc->GetTexHeight());
    const float newScale = static_cast<float>(mSize) / textureSize;
    SetActorScale(newScale);
    const float mRowOffset = mPosition.y * mSize + (textureSize / 2) * newScale;
    const float mColOffset = mPosition.x * mSize + (textureSize / 2) * newScale;
    SetActorLocation(Vector2(mColOffset, mRowOffset));
}

PieceColor Square::GetColor() const
{
    if (IsEmpty())
    {
        return PieceColor::Default_MAX;
    }
    else
    {
        return mPiece.lock()->GetColor();
    }
}

PieceType Square::GetTypeOfPiece() const
{
    auto currentPiece = mPiece.lock();
    // 현재 기물이 있는 경우
    if (currentPiece)
    {
        return currentPiece->GetType();
    }
    return PieceType::Default_MAX;
}

void Square::Occupied(const std::shared_ptr<Piece>& piece)
{
    mPiece = piece; 
}
void Square::Selected()
{
    mSeletedSprite.lock()->SetAble();
    mbSelected = true;
}
void Square::Unselected()
{
    mSeletedSprite.lock()->SetDisable();
    mbSelected = false;
}

void Square::BeCandidate()
{
    mCandidatedSprite.lock()->SetAble();
    mbCandidate = true;
}

void Square::CancelCandidate()
{
    mCandidatedSprite.lock()->SetDisable();
    mbCandidate = false;
}
void Square::BeAttackedBy(PieceColor color)
{
    if (color == PieceColor::Black)
    {
        mbAttackedByBlack = true;
#if DEBUGGING_ATTACK_SPRITE
        mAttackedByBlackSprite.lock()->SetAble();
#endif
    }
    else
    {
        mbAttackedByWhite = true;
#if DEBUGGING_ATTACK_SPRITE
        mAttackedByWhiteSprite.lock()->SetAble();
#endif
    }
}

void Square::ReleaseFromAttackBy(PieceColor color)
{
    if (color == PieceColor::White)
    {
        mbAttackedByWhite = false;
#if DEBUGGING_ATTACK_SPRITE
        mAttackedByWhiteSprite.lock()->SetDisable();
#endif
    }
    else
    {
        mbAttackedByBlack = false;
#if DEBUGGING_ATTACK_SPRITE
        mAttackedByBlackSprite.lock()->SetDisable();
#endif
    }
}
