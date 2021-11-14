#pragma once
#include <memory>
class Component : public std::enable_shared_from_this<Component>
{
public:
    // 생성자
    // (업데이트 순서값이 작을 수록 컴포넌트는 더 빨리 갱신된다.)
    Component(class Actor& owner, int updateOrder = 100);
    // 생성 이후에 초기화를 위한 initialze 함수. Actor::CreatComponent()에서 생성 직후 바로 호출됨
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    // 소멸자
    virtual ~Component();
    // 델타 시간으로 이 컴포넌트를 업데이트
    virtual void Update(float deltaTime) = 0;
    int GetUpdateOrder() const { return mUpdateOrder; }
protected:
    // 소유자 액터, 컴포넌트가 필요할 때 액터가 가진 다양한 데이터 및 정보에 접근하기 위해 필요.
    Actor& mOwner;
    // 컴포넌트의 업데이트 순서
    int mUpdateOrder;
};