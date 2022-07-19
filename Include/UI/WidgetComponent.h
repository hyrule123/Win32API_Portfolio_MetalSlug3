#pragma once
#include "WidgetWindow.h"

class CWidgetComponent :
    public CWidgetWindow
{
    friend class CScene;
    friend class CGameObject;

protected:
    CWidgetComponent();
    CWidgetComponent(const CWidgetComponent& widget);
    virtual ~CWidgetComponent();

public://�Ʒ� �޼ҵ���� �������ϰ� �θ��� �޼ҵ带 ȣ������ ���� ����. �浹ó�� X
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void Render(HDC hDC, float DeltaTime);

private:
    class CGameObject* m_OwnerGameObj;


protected:
    virtual bool CollisionMouse(CWidget** WidgetAddr, const Vector2& Mousepos);


};

