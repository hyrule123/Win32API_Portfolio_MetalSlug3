#pragma once
#include "Character.h"

class CBackModen :
    public CCharacter
{
	friend class CScene;
protected:
	CBackModen();
	CBackModen(const CBackModen& Obj);
	virtual ~CBackModen();

public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);

public:
	virtual int InflictDamage(int Damage);
};

