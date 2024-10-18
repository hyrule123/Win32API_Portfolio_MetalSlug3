#pragma once

#include "Projectile.h"

class CPistol :
	public CProjectile
{
	friend class CScene;


protected:
	CPistol();
	CPistol(const CPistol& Obj);
	virtual ~CPistol();

public:
	bool Init(CGameObject* Obj = nullptr);
	bool LoadResource();

	void CollisionBegin(class CCollider* Src, class CCollider* Dest);
};

