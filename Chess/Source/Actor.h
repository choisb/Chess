#pragma once
#include <memory>
#include <vector>
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
    // 액터에 부착된 모든 컴포넌트를 업데이트
    void UpdateComponents(float deltaTime);
    // 특정 액터에 특화된 업데이트 코드 (오버라이딩 가능)
    virtual void UpdateActor(float deltaTime);

    // Getter & Setter
    State GetState() const { return mState; }

private:
    // Game 객체
    Game& mGame;
    // Actor의 상태
    State mState;

    //std::vector<std::shared_ptr<class Component>> mComponents;
};

