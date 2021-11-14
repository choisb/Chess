#pragma once
#include <memory>
class Component : public std::enable_shared_from_this<Component>
{
public:
    // ������
    // (������Ʈ �������� ���� ���� ������Ʈ�� �� ���� ���ŵȴ�.)
    Component(class Actor& owner, int updateOrder = 100);
    // ���� ���Ŀ� �ʱ�ȭ�� ���� initialze �Լ�. Actor::CreatComponent()���� ���� ���� �ٷ� ȣ���
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;

    // �Ҹ���
    virtual ~Component();
    // ��Ÿ �ð����� �� ������Ʈ�� ������Ʈ
    virtual void Update(float deltaTime) = 0;
    int GetUpdateOrder() const { return mUpdateOrder; }
protected:
    // ������ ����, ������Ʈ�� �ʿ��� �� ���Ͱ� ���� �پ��� ������ �� ������ �����ϱ� ���� �ʿ�.
    Actor& mOwner;
    // ������Ʈ�� ������Ʈ ����
    int mUpdateOrder;
};