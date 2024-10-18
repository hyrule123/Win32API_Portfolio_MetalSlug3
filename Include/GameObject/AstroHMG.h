#pragma once
#include "Projectile.h"
class CAstroHMG :
	public CProjectile
{
	friend class CScene;

protected:
	CAstroHMG();
	CAstroHMG(const CAstroHMG& Obj);
	virtual ~CAstroHMG();

public:
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual bool LoadResource();
	virtual void SetEssential(Vector2 Dir, Vector2 Pos);



private:
	void CollisionBegin(class CCollider* Src, class CCollider* Dest);
};

