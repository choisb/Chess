#pragma once
class Component {
public:
    // ������
    // (������Ʈ �������� ���� ���� ������Ʈ�� �� ���� ���ŵȴ�.)
    Component(class Actor& owner, int updateOrder = 100);
    // �Ҹ���
    virtual ~Component();
    // ��Ÿ �ð����� �� ������Ʈ�� ������Ʈ
    virtual void Update(float deltaTime);
    int GetUpdateOrder() const { return mUpdateOrder; }
protected:
    // ������ ����, ������Ʈ�� �ʿ��� �� ���Ͱ� ���� �پ��� ������ �� ������ �����ϱ� ���� �ʿ�.
    Actor& mOwner;
    // ������Ʈ�� ������Ʈ ����
    int mUpdateOrder;
};