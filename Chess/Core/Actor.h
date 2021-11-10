#pragma once
#include <memory>
#include <vector>
#include <type_traits>
#include "Math.h"

class Actor : public std::enable_shared_from_this<Actor>
{
public:
    // Actor�� ����
    enum class State
    {
        EActive,
        EPaused,
        EDead,

        DEF_MAX
    };

    Actor(class Game& game);
    virtual ~Actor();

    // �������� ȣ��Ǵ� �ʱ�ȭ �Լ�
    virtual void Initialize() {};
    virtual void Shutdown();
    // Game���� ȣ���ϴ� Update �Լ�
    void Update(float deltaTime);
    // Actor�� ���ο� Component�� �����ϴ� �Լ�
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

template<class T, class... Param>
std::shared_ptr<T> Actor::CreateComponent(Param&&... _Args)
{
    // Component�� �Ļ�Ŭ������ �ƴҰ�� ����ó��
    static_assert(std::is_base_of<Component, T>::value, "Template argument T must be a derived class from the Component class");
    
    std::shared_ptr<T> component = std::make_shared<T>(std::forward<Param>(_Args)...);
    component->Initialize();
    AddComponentToArray(std::static_pointer_cast<Component>(component));
    return component;
}