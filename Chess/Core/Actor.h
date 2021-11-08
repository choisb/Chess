#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include "Math.h"

class Actor
{
public:
    // Actor의 상태
    enum State
    {
        EActive,
        EPaused,
        EDead,

        DEF_MAX
    };

    Actor(class Game& game);
    virtual ~Actor();

    // Game에서 호출하는 Update 함수
    void Update(float deltaTime);
    // Actor에 새로운 Component를 추가하는 함수
    template<class T>
    std::weak_ptr<T> CreateComponent();

    //// Getter & Setter
    Game& GetGame() const { return mGame; }
    State GetState() const { return mState; }
    Vector2 GetLocation() const { return mLocation; }
    float GetScale() const { return mScale; }
    float GetRotation() const { return mRotation; }

    void SetLocation(Vector2 inLocation) { mLocation = std::move(inLocation); }
    void SetScale(float inScale) { mScale = inScale; }
    void SetRotation(float inRotation) { mRotation = inRotation; }
protected:
    // 특정 액터에 특화된 업데이트 코드 (오버라이딩 가능)
    virtual void UpdateActor(float deltaTime);

private:
    // 액터에 부착된 모든 컴포넌트를 업데이트
    void UpdateComponents(float deltaTime);
    // 추가된 Component를 배열에추가하는 함수
    void AddComponentToArray(std::shared_ptr<class Component> component);
    // Game 객체
    Game& mGame;
    // Actor의 상태
    State mState;
    // Actor의 Transform
    Vector2 mLocation;
    float mScale;
    float mRotation;

    std::vector<std::shared_ptr<Component>> mComponents;
};

template<class T>
std::weak_ptr<T> Actor::CreateComponent()
{
    // Component의 파생클래스가 아닐경우 예외처리
    if (std::is_base_of<Component, T>::value == false)
        return std::weak_ptr<T>();
    
    std::shared_ptr<T> tSharedPtr = std::make_shared<T>(*this);
    std::weak_ptr<T> tWeakPtr = tSharedPtr;
    std::shared_ptr<Component> compSharedPtr = std::static_pointer_cast<Component>(tSharedPtr);
    compSharedPtr->Initialize();
    AddComponentToArray(compSharedPtr);
    return tWeakPtr;
}