#pragma once
#include <memory>
#include <vector>
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
    std::weak_ptr<class Component> AddComponent();

    //// Getter & Setter
    State GetState() const { return mState; }

protected:

    // 특정 액터에 특화된 업데이트 코드 (오버라이딩 가능)
    virtual void UpdateActor(float deltaTime);

private:
    // 액터에 부착된 모든 컴포넌트를 업데이트
    void UpdateComponents(float deltaTime);
    // 추가된 Component를 배열에추가하는 함수
    void AddComponentToArray(std::shared_ptr<Component> component);
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

