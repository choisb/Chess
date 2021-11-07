#pragma once
#include <memory>
#include <vector>
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
    std::weak_ptr<class Component> AddComponent();

    //// Getter & Setter
    State GetState() const { return mState; }

protected:

    // Ư�� ���Ϳ� Ưȭ�� ������Ʈ �ڵ� (�������̵� ����)
    virtual void UpdateActor(float deltaTime);

private:
    // ���Ϳ� ������ ��� ������Ʈ�� ������Ʈ
    void UpdateComponents(float deltaTime);
    // �߰��� Component�� �迭���߰��ϴ� �Լ�
    void AddComponentToArray(std::shared_ptr<Component> component);
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

