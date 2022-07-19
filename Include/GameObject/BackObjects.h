#pragma once
#include "Character.h"

//ÇÊ¼ö: SetObjType


enum class EBackObjType
{
	SmallAstro,
	BigAstro,
	LaserBack,
	LaserFront,
	DebrisSmall,
	DebrisBig,
	End
};

class CBackObjects :
    public CCharacter
{
	friend class CScene;

protected:
	CBackObjects();
	CBackObjects(const CBackObjects& Obj);
	virtual ~CBackObjects();

public:
	virtual bool LoadResource();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	virtual int InflictDamage(int Damage);



protected:
	EBackObjType m_BackObjType;
public:
	void SetEssential(EBackObjType Type);
	void CollisionBegin(class CCollider* Src, class CCollider* Dest);
};

