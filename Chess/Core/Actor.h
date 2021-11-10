#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include "Math.h"

class Actor : public std::enable_shared_from_this<Actor>
{
public:
    // Actor의 상태
    enum class State
    {
        EActive,
        EPaused,
        EDead,

        DEF_MAX
    };

    Actor(class Game& game);
    virtual ~Actor();

    // 생성직후 호출되는 초기화 함수
    virtual void Initialize() {};
    virtual void Shutdown();
    // Game에서 호출하는 Update 함수
    void Update(float deltaTime);
    // Actor에 새로운 Component를 생성하는 함수
    template<class T, class... Param>
    std::shared_ptr<T> CreateComponent(Param&&... _Val);

    //// Getter & Setter
    Game& GetGame() const { return mGame; }
    State GetState() const { return mState; }
    Vector2 GetActorLocation() const { return mLocation; }
    float GetActorScale() const { return mScale; }
    float GetActorRotation() const { return mRotation; }

    void SetActorLocation(Vector2 inLocation) { mLocation = std::move(inLocation); }
    void SetActorScale(float inScale) { mScale = inScale; }
    void SetActorRotation(float inRotation) { mRotation = inRotation; }
    void SetState(State inState) { mState = inState; }
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

template<class T, class... Param>
std::shared_ptr<T> Actor::CreateComponent(Param&&... _Args)
{
    // Component의 파생클래스가 아닐경우 예외처리
    static_assert(std::is_base_of<Component, T>::value, "Template argument T must be a derived class from the Component class");
    
    std::shared_ptr<T> component = std::make_shared<T>(std::forward<Param>(_Args)...);
    component->Initialize();
    AddComponentToArray(std::static_pointer_cast<Component>(component));
    return component;
}