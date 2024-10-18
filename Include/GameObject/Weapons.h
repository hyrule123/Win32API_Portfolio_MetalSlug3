#pragma once
#include "Effect.h"

class CWeapons :
	public CEffect
{
	friend class CScene;

protected:
	CWeapons();
	CWeapons(const CWeapons& Obj);
	virtual ~CWeapons();

public:
	virtual bool Init(CGameObject* Obj = nullptr);


protected:
	virtual bool LoadResource();

public:
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
};

