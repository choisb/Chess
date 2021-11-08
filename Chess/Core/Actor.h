#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include "Math.h"

class Actor
{
public:
    // Actor�� ����
    enum State
    {
        EActive,
        EPaused,
        EDead,

        DEF_MAX
    };

    Actor(class Game& game);
    virtual ~Actor();

    // Game���� ȣ���ϴ� Update �Լ�
    void Update(float deltaTime);
    // Actor�� ���ο� Component�� �߰��ϴ� �Լ�
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
    // Ư�� ���Ϳ� Ưȭ�� ������Ʈ �ڵ� (�������̵� ����)
    virtual void UpdateActor(float deltaTime);

private:
    // ���Ϳ� ������ ��� ������Ʈ�� ������Ʈ
    void UpdateComponents(float deltaTime);
    // �߰��� Component�� �迭���߰��ϴ� �Լ�
    void AddComponentToArray(std::shared_ptr<class Component> component);
    // Game ��ü
    Game& mGame;
    // Actor�� ����
    State mState;
    // Actor�� Transform
    Vector2 mLocation;
    float mScale;
    float mRotation;

    std::vector<std::shared_ptr<Component>> mComponents;
};

template<class T>
std::weak_ptr<T> Actor::CreateComponent()
{
    // Component�� �Ļ�Ŭ������ �ƴҰ�� ����ó��
    if (std::is_base_of<Component, T>::value == false)
        return std::weak_ptr<T>();
    
    std::shared_ptr<T> tSharedPtr = std::make_shared<T>(*this);
    std::weak_ptr<T> tWeakPtr = tSharedPtr;
    std::shared_ptr<Component> compSharedPtr = std::static_pointer_cast<Component>(tSharedPtr);
    compSharedPtr->Initialize();
    AddComponentToArray(compSharedPtr);
    return tWeakPtr;
}