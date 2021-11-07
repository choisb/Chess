#pragma once
#include <memory>
#include <vector>
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
    // ���Ϳ� ������ ��� ������Ʈ�� ������Ʈ
    void UpdateComponents(float deltaTime);
    // Ư�� ���Ϳ� Ưȭ�� ������Ʈ �ڵ� (�������̵� ����)
    virtual void UpdateActor(float deltaTime);

    // Getter & Setter
    State GetState() const { return mState; }

private:
    // Game ��ü
    Game& mGame;
    // Actor�� ����
    State mState;

    //std::vector<std::shared_ptr<class Component>> mComponents;
};

