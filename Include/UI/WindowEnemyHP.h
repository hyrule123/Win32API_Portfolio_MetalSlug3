#pragma once
#include "WidgetWindow.h"

#include "../GameObject/Enemy.h"


class CWindowEnemyHP :
    public CWidgetWindow
{
    friend class CScene;

protected:
    CWindowEnemyHP();
    CWindowEnemyHP(const CWindowEnemyHP& widget);
    virtual ~CWindowEnemyHP();
public:
    virtual bool Init();

private:
    class CWidgetProgressBar* m_Bar;
    class CWidgetNumber* m_Number;


    class CEnemy* m_Enemy;
    float m_EnemyHPMax; //�ʱ�ȭ�� �ٷ� ������ HP�� �޾ƿͼ� �װ� ���Ƿ� ����

    //������ ��ġ���� �⺻������ �󸶳� ����߷��� �������
    //(ü�¹��̹Ƿ�)
    Vector2 m_Offset;
public:
    void SetOffset(const Vector2& Offset);

public:
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);

};

