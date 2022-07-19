#pragma once

#include "GameObject.h"




class CCharacter :
    public CGameObject
{
	friend class CScene;

protected:

	int m_HP;




protected:
	CCharacter();
	CCharacter(const CCharacter& Obj);
	virtual ~CCharacter();

public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);



public:
	virtual int InflictDamage(int Damage);
};

