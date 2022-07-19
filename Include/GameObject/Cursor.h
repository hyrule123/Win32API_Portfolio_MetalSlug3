#pragma once
#include "GameObject.h"


class CCursor :
    public CGameObject
{
	friend class CScene;

protected:
	CCursor();
	CCursor(const CCursor& Obj);
	virtual ~CCursor();


public:
	virtual bool Init(CGameObject* Obj = nullptr);

protected:
	class CColliderPoint* m_MouseCollider;


public:
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

};


