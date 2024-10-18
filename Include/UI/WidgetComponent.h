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

public://아래 메소드들은 재정의하고 부모의 메소드를 호출하지 않을 것임. 충돌처리 X
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

private:
	class CGameObject* m_OwnerGameObj;


protected:
	virtual bool CollisionMouse(CWidget** WidgetAddr, const Vector2& Mousepos);


};

